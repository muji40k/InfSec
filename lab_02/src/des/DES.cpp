#include "DES.h"

#include <cstring>

FBlock::FBlock(std::shared_ptr<EBlock> e_block,
               std::vector<std::shared_ptr<SBlock>> s_blocks,
               std::shared_ptr<FPBlock> p_block)
    : e_block(e_block), s_blocks(s_blocks), p_block(p_block)
{
    if (nullptr == e_block || nullptr == p_block)
        throw;

    for (auto block : s_blocks)
        if (nullptr == block)
            throw;
}

void FBlock::apply(BitRange in, BitRange key, BitRange out)
{
    if (32 != in.size() || 48 != key.size() || 32 != out.size())
        throw;

    char buffer[6], out_buffer[4];
    BitRange buf_rng (buffer, 0, 48), out_rng (out_buffer, 0, 32);
    auto splits_in = buf_rng.split(8);
    auto splits_out = out_rng.split(8);

    this->e_block->direct(in, buf_rng);
    buf_rng ^= key;

    for (size_t i = 0; 8 > i; i++)
        this->s_blocks[i]->direct(splits_in[i], splits_out[i]);

    this->p_block->direct(out_rng, out);
}

KeyBlock::KeyBlock(BitRange key, std::shared_ptr<PKeyBlock> p_block,
                   std::vector<std::shared_ptr<ShiftKeyBlock>> shift_blocks,
                   std::shared_ptr<HKeyBlock> h_block)
    : p_block(p_block), shift_blocks(shift_blocks), h_block(h_block)
{
    if (64 != key.size())
        throw;

    if (nullptr == p_block || nullptr == h_block)
        throw;

    for (auto block : shift_blocks)
        if (nullptr == block)
            throw;

    char buf[7];
    BitRange inner (buf, 0, 56);

    if (!this->check(key, inner))
        throw;

    this->init(inner);
}

void KeyBlock::get(size_t i, BitRange out)
{
    if (16 <= i)
        throw;

    BitRange tmp (this->keys[i], 0, 48);
    out.copy(tmp);
}

bool KeyBlock::check(BitRange key, BitRange out)
{
    bool k = true;
    char sum = 0;
    auto iterk = key.begin(), itero = out.begin();

    for (size_t i = 0; 8 > i; i++, ++iterk)
    {
        sum = 0;

        for (size_t j = 0; 7 > j; j++, ++itero, ++iterk)
            sum += (*itero = *iterk) ? 1 : 0;

        if (sum % 2 != ((*iterk) ? 1 : 0))
            k = false;
    }

    return k;
}

void KeyBlock::init(BitRange key)
{
    if (56 != key.size())
        throw;

    char buffer[7], tmp[6];
    BitRange buf_rng (buffer, 0, 56), shift_rng (tmp, 0, 28), out_rng (tmp, 0, 48);
    auto splits_buf = buf_rng.split(2);
    BitRange c = splits_buf[0], d = splits_buf[1];

    this->p_block->direct(key, c, d);

    for (size_t i = 0; 16 > i; i++)
    {
        this->shift_blocks[i]->direct(c, shift_rng);
        c.copy(shift_rng);
        this->shift_blocks[i]->direct(d, shift_rng);
        d.copy(shift_rng);

        this->h_block->direct(buf_rng, out_rng);

        memmove(this->keys[i], tmp, 6);
    }
}

void KeyBlock::getKey(BitRange in, BitRange out)
{
    if (64 != in.size() || 56 != out.size())
        throw;

    bool valid = true;
    auto iter_in = in.begin(), iter_out = out.begin();

    for (size_t i = 0; valid && 8 > i; i++, ++iter_in)
    {
        int sum = 0;

        for (size_t j = 0; valid && 7 > j; j++, ++iter_out, ++iter_in)
            sum += (*iter_out = *iter_in) ? 1 : 0;

        if ((*iter_in ? 1 : 0) != sum % 2)
            valid = false;
    }

    if (!valid)
        throw;
}

DES::DES(std::shared_ptr<IPBlock> ip_block,
         std::shared_ptr<FBlock> f_block,
         std::shared_ptr<KeyBlock> key_block)
    : ip_block(ip_block), f_block(f_block), key_block(key_block)
{
    if (nullptr == ip_block || nullptr == f_block || nullptr == key_block)
        throw;
}

std::string DES::encode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    size_t rest = length % 8;

    if (0 != rest)
        length += 8 - rest;

    std::string out (length, 0);
    char *buffer = out.data();

    memmove(buffer, origin.c_str(), origin.length());

    BitRange main_rng (buffer, 0, length * 8);

    auto msgs = main_rng.splitSize(64);

    for (size_t i = 0; msgs.size() > i; i++)
        this->encodeBlock(msgs[i]);

    return out;
}

std::string DES::decode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    if (0 != length % 8)
        throw;

    std::string out (length, 0);
    char *buffer = out.data();

    memmove(buffer, origin.c_str(), length);

    BitRange main_rng (buffer, 0, length * 8);

    auto msgs = main_rng.splitSize(64);

    for (size_t i = 0; msgs.size() > i; i++)
        this->decodeBlock(msgs[i]);

    return out;
}

void DES::encodeBlock(BitRange current)
{
    if (64 != current.size())
        throw;

    char buffer[8], key_buffer[6];
    BitRange buf_rng (buffer, 0, 64), key_rng (key_buffer, 0, 48),
             buf_32(buffer, 0, 32);

    this->ip_block->direct(current, buf_rng);
    current.copy(buf_rng);
    auto splits = current.split(2);
    BitRange l = splits[0], r = splits[1];

    for (size_t i = 0; 16 > i; i++)
    {
        this->key_block->get(i, key_rng);
        this->f_block->apply(r, key_rng, buf_32);
        buf_32 ^= l;
        l.copy(r);
        r.copy(buf_32);
    }

    this->ip_block->reverse(current, buf_rng);
    current.copy(buf_rng);
}

void DES::decodeBlock(BitRange current)
{
    if (64 != current.size())
        throw;

    char buffer[8], key_buffer[6];
    BitRange buf_rng (buffer, 0, 64), key_rng (key_buffer, 0, 48),
             buf_32(buffer, 0, 32);

    this->ip_block->direct(current, buf_rng);
    current.copy(buf_rng);
    auto splits = current.split(2);
    BitRange l = splits[0], r = splits[1];

    for (size_t i = 0; 16 > i; i++)
    {
        this->key_block->get(15 - i, key_rng);
        this->f_block->apply(l, key_rng, buf_32);
        buf_32 ^= r;
        r.copy(l);
        l.copy(buf_32);
    }

    this->ip_block->reverse(current, buf_rng);
    current.copy(buf_rng);
}

