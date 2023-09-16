#ifndef _ALPHABET_H_
#define _ALPHABET_H_

#include <cstddef>

template <typename CharType>
class Alphabet
{
    public:
        virtual ~Alphabet(void) = default;
        virtual size_t size(void) const = 0;
        virtual CharType letter(size_t c) = 0;
        virtual size_t order(CharType c) = 0;
};

#endif

