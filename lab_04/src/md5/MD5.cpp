#include "MD5.h"

#include <md5.h>

const unsigned char MD5::expansion[64] = { 0x80 };

const size_t MD5::S[64] =
{
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

const unsigned int MD5::K[64] =
{
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

template <typename Type>
Type shift_left(const Type val, size_t size)
{
    static constexpr const size_t tsize = sizeof(Type) << 3;
    size %= tsize;

    return (val << size) | (val >> (tsize - size));
}

std::string MD5::apply(const std::string &origin)
{
    size_t size = origin.length(), esize;
    const size_t bits = size * 8;
    const size_t rest = size % 64;

    if (56 > rest)
        esize = 56 - rest;
    else
        esize = 120 - rest;

    size += esize + 8;
    std::string copy;
    copy.reserve(size);

    copy += origin + std::string((const char *)MD5::expansion, esize)
            + std::string((char *)&bits, 8);

    const unsigned char *data = (unsigned char *)copy.data();
    const unsigned int *current = nullptr;
    unsigned int A = 0x67452301, a,
                 B = 0xEFCDAB89, b,
                 C = 0x98BADCFE, c,
                 D = 0x10325476, d,
                 F = 0;
    Iter handler;

    for (size_t i = 0, blocks = size >> 6; blocks > i; i++)
    {
        a = A, b = B, c = C, d = D;
        current = (const unsigned int *)(data + i * 64);

        for (size_t j = 0; 64 > j; j++)
        {
            MD5::getIteration(j, handler);

            F = handler.func(b, c, d) + a + MD5::K[j] + current[handler.k];
            a = d;
            d = c;
            c = b;
            b += shift_left<unsigned int>(F, MD5::S[j]);
        }

        A += a, B += b, C += c, D += d;
    }

    return std::string((char *)&A, 4) + std::string((char *)&B, 4)
           + std::string((char *)&C, 4) + std::string((char *)&D, 4);
}

void MD5::getIteration(const size_t i, Iter &handler)
{
    if (16 > i)
    {
        handler.func = &MD5::F;
        handler.k = i;
    }
    else if (32 > i)
    {
        handler.func = &MD5::G;
        handler.k = (5 * i + 1) % 16;
    }
    else if (48 > i)
    {
        handler.func = &MD5::H;
        handler.k = (3 * i + 5) % 16;
    }
    else
    {
        handler.func = &MD5::I;
        handler.k = (7 * i) % 16;
    }
}

unsigned int MD5::F(const unsigned int b, const unsigned int c, const unsigned int d)
{
    return (b & c) | (~b & d);
}

unsigned int MD5::G(const unsigned int b, const unsigned int c, const unsigned int d)
{
    return (b & d) | (~d & c);
}

unsigned int MD5::H(const unsigned int b, const unsigned int c, const unsigned int d)
{
    return b ^ c ^ d;
}

unsigned int MD5::I(const unsigned int b, const unsigned int c, const unsigned int d)
{
    return c ^ (~d | b);
}

