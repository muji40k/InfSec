#include "RSA.h"

static unsigned long long modadd(unsigned long long a, unsigned long long b,
                                 unsigned long long mod);
static unsigned long long modmul(unsigned long long a, unsigned long long b,
                                 unsigned long long mod);
static unsigned long long modpow(unsigned long long a, unsigned long long pow,
                                 unsigned long long mod);

RSA::RSA(unsigned long long n)
    : n(n)
{
    if (0 == n)
        return;
}

void RSA::setPublicKey(unsigned long long e)
{
    this->e = e;
}

void RSA::setPrivateKey(unsigned long long d)
{
    this->d = d;
}

std::string RSA::encode(const std::string &origin)
{
    if (0 == this->e)
        throw;

    size_t size = origin.length(), rest = size % 7;
    size = size / 7 + (rest ? 1 : 0);

    std::string out;
    out.reserve(size * 8);

    unsigned long long current, mask = 0x00ffffffffffffff;

    for (size_t i = 0; size > i; i++)
    {
        if (size == i + 1 && rest)
        {
            mask = 0;

            for (size_t j = 0; rest > j; j++)
                mask = (mask << 8) | 0xff;
        }

        current = *(unsigned long long *)(origin.data() + 7 * i) & mask;
        current = modpow(current, this->e, this->n);
        out += std::string((char *)&current, 8);
    }

    return out;
}

std::string RSA::decode(const std::string &origin)
{
    if (0 == this->d)
        throw;

    size_t size = origin.length(), psize = size / 8;

    if (0 != size % 8)
        throw;

    std::string out;
    out.reserve(size - psize);
    out.resize(size - psize);

    const unsigned long long *inp = (unsigned long long *)origin.data();
    unsigned long long current;
    char *outp = out.data(), *currentp = (char *)&current;

    for (size_t i = 0; psize > i; i++)
    {
        current = inp[i];
        current = modpow(current, this->d, this->n);

        for (size_t j = 0; 7 > j; j++)
            *(outp++) = currentp[j];
    }

    return out;
}

static unsigned long long modadd(unsigned long long a, unsigned long long b,
                                 unsigned long long mod)
{
    a %= mod;
    b %= mod;

    unsigned long long diff = mod - b;

    if (a >= diff)
        return a - diff;

    return a + b;
}

static unsigned long long modmul2(unsigned long long a, unsigned long long mod)
{
    unsigned long long shrinked = mod >> 1, rest = mod & 1;

    if (a >= shrinked)
        return ((a - shrinked) << 1) - rest;

    return a << 1;
}

static unsigned long long modmul(unsigned long long a, unsigned long long b,
                                 unsigned long long mod)
{
    a %= mod;
    b %= mod;

    unsigned long long result = 0;

    for (; b; b >>= 1, a = modmul2(a, mod))
        if (b & 1)
            result = modadd(result, a, mod);

    return result;
}

static unsigned long long modpow(unsigned long long a, unsigned long long pow,
                                 unsigned long long mod)
{
    a %= mod;

    unsigned long long result = 1;

    for (; pow; pow >>= 1)
    {
        if (pow & 1)
            result = modmul(result, a, mod);

        a = modmul(a, a, mod);
    }

    return result;
}

