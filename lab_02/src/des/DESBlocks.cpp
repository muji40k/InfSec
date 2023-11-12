#include "DESBlocks.h"

BasicSBlock::BasicSBlock(std::array<std::array<char, 16>, 4> table)
    : table(table)
{}

void BasicSBlock::direct(BitRange in, BitRange out)
{
    if (6 != in.size() || 4 != out.size())
        throw;

    int row = 2 * (in[0] ? 1 : 0) + 1 * (in[5] ? 1 : 0);
    int column = 0;
    int power = 1;
    auto iter = in.begin();
    ++iter;

    for (size_t i = 0; 4 > i; i++, ++iter, power <<= 1)
        column += power * (**iter ? 1 : 0);

    char val = this->table[row][column];
    BitRange rng (&val, 0, 4);
    out.copy(rng);
}

BasicFPBlock::BasicFPBlock(std::shared_ptr<BitPermutator> permutator)
    : permutator(permutator)
{
    if (nullptr == permutator)
        throw;
}

void BasicFPBlock::direct(BitRange in, BitRange out)
{
    if (32 != in.size() || 32 != out.size())
        throw;

    for (Bit bit : out)
        bit = in[this->permutator->direct(bit.index())];
}

void BasicEBlock::direct(BitRange in, BitRange out)
{
    if (32 != in.size() || 48 != out.size())
        throw;

    for (size_t i = 0, j, k; 8 > i; i++)
    {
        j = 6 * i, k = 4 * i;
        out[j]     = in[!k ? 31 : k - 1];
        out[j + 1] = in[k];
        out[j + 2] = in[k + 1];
        out[j + 3] = in[k + 2];
        out[j + 4] = in[k + 3];
        out[j + 5] = in[7 == i ? 0 : k + 4];
    }
}

BasicPKeyBlock::BasicPKeyBlock(std::shared_ptr<BitPermutator> c_perm,
                               std::shared_ptr<BitPermutator> d_perm)
    : c_perm(c_perm), d_perm(d_perm)
{
    if (nullptr == c_perm || nullptr == d_perm)
        throw;
}

void BasicPKeyBlock::direct(BitRange in, BitRange c, BitRange d)
{
    if (56 != in.size() || 28 != c.size() || 28 != d.size())
        throw;

    for (Bit bit : c)
        bit = in[this->c_perm->direct(bit.index())];

    for (Bit bit : d)
        bit = in[this->d_perm->direct(bit.index())];
}

BasicHKeyBlock::BasicHKeyBlock(std::shared_ptr<BitPermutator> permutator)
    : permutator(permutator)
{
    if (nullptr == permutator)
        throw;
}

void BasicHKeyBlock::direct(BitRange in, BitRange out)
{
    if (56 != in.size() || 48 != out.size())
        throw;

    for (Bit bit : out)
        bit = in[this->permutator->direct(bit.index())];
}

BasicShiftKeyBlock::BasicShiftKeyBlock(std::shared_ptr<BitShifter> shifter)
    : shifter(shifter)
{
    if (nullptr == shifter)
        throw;
}

void BasicShiftKeyBlock::direct(BitRange in, BitRange out)
{
    if (28 != in.size() || 28 != out.size())
        throw;

    for (Bit bit : out)
        bit = in[this->shifter->direct(bit.index())];
}

BasicIPBlock::BasicIPBlock(std::shared_ptr<BitPermutator> permutator)
    : permutator(permutator)
{
    if (nullptr == permutator)
        throw;
}

void BasicIPBlock::direct(BitRange in, BitRange out)
{
    if (64 != in.size() || 64 != out.size())
        throw;

    for (Bit bit : out)
        bit = in[this->permutator->direct(bit.index())];
}

void BasicIPBlock::reverse(BitRange in, BitRange out)
{
    if (64 != in.size() || 64 != out.size())
        throw;

    for (Bit bit : out)
        bit = in[this->permutator->reverse(bit.index())];
}

