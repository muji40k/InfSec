#include "BitPermutator.h"

BitTable::BitTable(std::vector<size_t> map, size_t size)
    : map(std::vector<size_t>(size)),
      inverse(std::vector<size_t>(size))
{
    for (size_t i = 0; map.size() > i; i++)
    {
        if (map[i] >= size)
            throw;

        this->map[i] = map[i];
        this->inverse[map[i]] = i;
    }
}

size_t BitTable::direct(const size_t in) const
{
    if (this->map.size() <= in)
        return in;

    return this->map[in];
}

size_t BitTable::reverse(const size_t in) const
{
    if (this->map.size() <= in)
        return in;

    return this->inverse[in];
}

BitShifter::BitShifter(size_t size, size_t shift)
    : size(size), shift(shift % size)
{
    if (0 == size)
        throw;
}

size_t BitShifter::direct(const size_t in) const
{
    return (in + this->shift) % this->size;
}

size_t BitShifter::reverse(const size_t in) const
{
    size_t out = in % this->size;

    if (this->shift > out)
        return size - this->shift + out;

    return out - this->shift;
}

