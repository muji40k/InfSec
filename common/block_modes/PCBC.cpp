#include "PCBC.h"

#include <cstring>

#include "BitRange.h"

PCBC::PCBC(std::list<std::shared_ptr<Encoder>> encoders,
           std::string initial_value, const size_t block_size)
    : encoders(encoders), initial_value(initial_value), block_size(block_size)
{
    if (initial_value.length() != block_size)
        throw;

    for (auto ptr : encoders)
        if (nullptr == ptr)
            throw;
}

std::string PCBC::encode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    size_t rest = length % this->block_size;

    if (0 != rest)
        length += this->block_size - rest;

    std::string out, cur_str;
    char cary_buf[this->block_size], tmp_buf[this->block_size];
    out.reserve(length);
    cur_str.reserve(this->block_size);
    cur_str.resize(this->block_size);

    memmove(cary_buf, this->initial_value.c_str(), this->block_size);

    BitRange cary (cary_buf, 0, this->block_size * 8);
    BitRange tmp (tmp_buf, 0, this->block_size * 8);
    BitRange current (cur_str.data(), 0, this->block_size * 8);
    auto iter = this->encoders.begin();

    for (size_t i = 0; length > i; i += this->block_size, ++iter)
    {
        if (this->encoders.end() == iter)
            iter = this->encoders.begin();

        std::string c = origin.substr(i, this->block_size);
        c.copy(cur_str.data(), c.length());

        if (this->block_size != c.length())
            for (size_t i = c.length(); this->block_size > i; cur_str[i++] = 0);

        memmove(tmp_buf, cur_str.data(), this->block_size);

        current ^= cary;
        std::string enc_str = (*iter)->encode(cur_str);

        BitRange enc_rng (enc_str.data(), 0, this->block_size * 8);
        tmp ^= enc_rng;

        memmove(cary_buf, tmp_buf, this->block_size);

        out += enc_str;
    }

    return out;
}

std::string PCBC::decode(const std::string &origin)
{
    size_t length = origin.length();

    if (0 == length)
        return origin;

    if (0 != length % this->block_size)
        throw;

    std::string out, cur_str(this->block_size, 0);
    char cary_buf[this->block_size], tmp_buf[this->block_size];
    out.reserve(length);

    memmove(cary_buf, this->initial_value.c_str(), this->block_size);

    BitRange cary (cary_buf, 0, this->block_size * 8);
    BitRange tmp (tmp_buf, 0, this->block_size * 8);
    BitRange current (cur_str.data(), 0, this->block_size * 8);
    auto iter = this->encoders.begin();

    for (size_t i = 0; origin.length() > i; i += this->block_size, ++iter)
    {
        if (this->encoders.end() == iter)
            iter = this->encoders.begin();

        cur_str = origin.substr(i, this->block_size);
        memmove(tmp_buf, cur_str.data(), this->block_size);

        std::string enc_str = (*iter)->decode(cur_str);
        BitRange enc_rng (enc_str.data(), 0, this->block_size * 8);

        enc_rng ^= cary;
        tmp ^= enc_rng;

        memmove(cary_buf, tmp_buf, this->block_size);

        out += enc_str;
    }

    return out;
}

