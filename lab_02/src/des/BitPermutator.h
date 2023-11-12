#ifndef _BITPERMUTATOR_H_
#define _BITPERMUTATOR_H_

#include <vector>
// #include <unordered_map>

class BitPermutator
{
    public:
        virtual ~BitPermutator(void) = default;
        virtual size_t direct(const size_t in) const = 0;
        virtual size_t reverse(const size_t in) const = 0;
};

class BitTable : public BitPermutator
{
    public:
        BitTable(std::vector<size_t> map, size_t size);
        virtual ~BitTable(void) override = default;
        virtual size_t direct(const size_t in) const override;
        virtual size_t reverse(const size_t in) const override;

    private:
        std::vector<size_t> map;
        std::vector<size_t> inverse;
};

class BitShifter : public BitPermutator
{
    public:
        BitShifter(size_t size, size_t shift);
        virtual ~BitShifter(void) override = default;
        virtual size_t direct(const size_t in) const override;
        virtual size_t reverse(const size_t in) const override;

    private:
        const size_t size;
        const size_t shift;
};

#endif

