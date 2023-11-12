#include "BitRange.h"

#include <memory>
#include <algorithm>

Bit::Bit(char *const source, unsigned char mask, size_t index)
    : source(source), mask(mask), bindex(index)
{}

Bit &Bit::operator = (bool state)
{
    this->set(state);

    return *this;
}

Bit &Bit::operator = (Bit other)
{
    this->set(*other);

    return *this;
}

bool Bit::operator * (void) const
{
    return this->get();
}

Bit::operator bool (void) const
{
    return this->get();
}

Bit &Bit::operator ^= (Bit other)
{
    if (other)
    {
        if (*this)
            this->set(false);
        else
            this->set(true);
    }

    return *this;
}

bool Bit::get(void) const
{
    return 0 < (*this->source & mask);
}

void Bit::set(bool state)
{
    if (state)
        *this->source |= this->mask;
    else
        *this->source &= ~this->mask;
}

size_t Bit::index(void) const
{
    return this->bindex;
}

BitRange::Iterator::Iterator(char *const source, char bit_offset, size_t bit_size, size_t start)
    : base(source), base_offset(bit_offset)
{
    if (bit_size < start)
        throw;

    size_t real = start + bit_offset;
    this->source = source + real / 8;
    this->mask = 1 << (real % 8);
    this->left = bit_size - start;
    this->index = start;
}

Bit BitRange::Iterator::operator * (void)
{
    return Bit(this->source, this->mask, this->index);
}

void BitRange::Iterator::operator ++ (void)
{
    if (!this->left)
        return;

    if (128 != this->mask)
        this->mask <<= 1;
    else
    {
        this->mask = 1;
        this->source++;
    }

    this->left--;
    this->index++;
}

bool BitRange::Iterator::operator != (Iterator &iter)
{
    if (this->base != iter.base || this->base_offset != iter.base_offset)
        return true;

    return this->left != iter.left || this->index != iter.index;
}

BitRange::Split::Split(BitRange &rng, size_t pieces)
    : rng(rng), pieces(pieces), part_size(rng.bit_size / pieces)
{}

size_t BitRange::Split::size(void) const
{
    return this->pieces;
}

 BitRange BitRange::Split::operator [] (size_t i)
{
    return this->at(i);
}

BitRange BitRange::Split::at(size_t i)
{
    const size_t real = this->rng.bit_offset + i * this->part_size;
    const size_t index = real / 8;
    const size_t offset = real % 8;

    return BitRange(this->rng.source + index, offset, this->part_size);
}

BitRange::BitRange(char *const source, char bit_offset, size_t bit_size)
    : source(source), bit_offset(bit_offset), bit_size(bit_size)
{}

size_t BitRange::size(void)
{
    return this->bit_size;
}

void BitRange::copy(BitRange other)
{
    const size_t bsize = std::min(this->bit_size, other.bit_size);
    auto iter1 = this->begin(), iter2 = other.begin();

    for (size_t i = 0; bsize > i; i++, ++iter1, ++iter2)
        (*iter1) = (*iter2);
}

Bit BitRange::operator [] (size_t offset)
{
    return this->at(offset);
}

Bit BitRange::at(size_t offset)
{
    Current c = this->get(offset);

    if (nullptr == c.dst)
        throw;

    return Bit(c.dst, 1 << c.offset, offset);
}

void BitRange::set(size_t offset, bool value)
{
    Current c = this->get(offset);

    if (nullptr == c.dst)
        return;

    if (value)
        *c.dst |= 1 << c.offset;
    else
        *c.dst &= ~(1 << c.offset);
}

BitRange &BitRange::operator ^= (BitRange other)
{
    if (this->bit_offset == other.bit_offset)
        return this->fastXOR(other);

    return this->regularXOR(other);
}

BitRange::Iterator BitRange::begin(void)
{
    return BitRange::Iterator(this->source, this->bit_offset, this->bit_size, 0);
}

BitRange::Iterator BitRange::end(void)
{
    return BitRange::Iterator(this->source, this->bit_offset, this->bit_size, this->bit_size);
}

BitRange::Split BitRange::split(size_t pieces)
{
    return Split(*this, pieces);
}

BitRange::Split BitRange::splitSize(size_t size)
{
    return Split(*this, this->bit_size / size);
}

BitRange::Current BitRange::get(size_t offset)
{
    Current out = {nullptr, 0};

    if (this->bit_size <= offset)
        return out;

    size_t real = this->bit_offset + offset;
    out.dst = this->source + real / 8;
    out.offset = real % 8;

    return out;
}

BitRange &BitRange::fastXOR(BitRange &other)
{
    size_t bsize = std::min(this->bit_size, other.bit_size);
    size_t limit = (bsize + this->bit_offset) / 8;
    char rest = (bsize + this->bit_offset) % 8;

    char mask = bit_offset ? 1 : 0;

    for (char i = 0; this->bit_offset - 1 > i; i++)
    {
        mask <<= 1;
        mask |= 1;
    }

    this->source[0] ^= other.source[0] & (~mask);

    if (!limit--)
        return *this;

    for (size_t i = 1; limit >= i; i++)
        this->source[i] ^= other.source[i];

    if (!rest)
        return *this;

    mask = rest ? 1 : 0;

    for (char i = 1; rest > i; i++)
    {
        mask <<= 1;
        mask |= 1;
    }

    this->source[limit + 1] ^= other.source[limit + 1] & mask;

    return *this;
}

BitRange &BitRange::regularXOR(BitRange &other)
{
    const size_t bsize = std::min(this->bit_size, other.bit_size);
    auto iter1 = this->begin(), iter2 = other.begin();

    for (size_t i = 0; bsize > i; i++, ++iter1, ++iter2)
        (*iter1) ^= (*iter2);

    return *this;
}

