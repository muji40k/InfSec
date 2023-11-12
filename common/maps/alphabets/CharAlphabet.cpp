#include "CharAlphabet.h"

#include <climits>

size_t CharAlphabet::size(void) const
{
    static const size_t size = 1 << CHAR_BIT;

    return size;
}

char CharAlphabet::letter(size_t c)
{
    c %= this->size();
    char out;

    if (c <= CHAR_MAX)
        out = c;
    else
    {
        c -= CHAR_MAX + 1;
        out = CHAR_MIN + c;
    }

    return out;
}

size_t CharAlphabet::order(char c)
{
    if (c >= 0)
        return c;

    return this->size() + c;
}

