#ifndef _BITRANGE_H_
#define _BITRANGE_H_

#include <cstddef>

class Bit
{
    public:
        Bit(char *const source, unsigned char mask, size_t index);

        Bit &operator = (bool state);
        Bit &operator = (Bit other);
        bool operator * (void) const; operator bool (void) const;
        Bit &operator ^= (Bit other);

        bool get(void) const;
        void set(bool state=true);

        size_t index(void) const;

    private:
        char *const source;
        const unsigned char mask;
        size_t bindex;
};

class BitRange
{
    public:
        class Iterator
        {
            public:
                Iterator(char *const source, char bit_offset, size_t bit_size, size_t start);

                Bit operator * (void);
                void operator ++ (void);
                bool operator != (Iterator &iter);

            private:
                const char *const base;
                const size_t base_offset;
                size_t index = 0;
                char *source;
                size_t left;
                unsigned char mask;
        };

        class Split
        {
            public:
                Split(BitRange &rng, size_t pieces);

                size_t size(void) const;

                BitRange operator [] (size_t i);
                BitRange at(size_t i);

            private:
                BitRange &rng;
                const size_t pieces;
                const size_t part_size;
        };

    private:
        using Current = struct
        {
            char *dst;
            unsigned char offset;
        };

    public:
        BitRange(char *const source, char bit_offset, size_t bit_size);

        size_t size(void);
        void copy(BitRange other);

        Bit operator [] (size_t offset);
        Bit at(size_t offset);
        void set(size_t offset, bool value=true);

        BitRange &operator ^= (BitRange other);

        BitRange::Iterator begin(void);
        BitRange::Iterator end(void);

        Split split(size_t pieces);
        Split splitSize(size_t size);

    private:
        Current get(size_t offset);
        BitRange &fastXOR(BitRange &other);
        BitRange &regularXOR(BitRange &other);

    private:
        char *const source;
        const char bit_offset;
        const size_t bit_size;
};

#endif

