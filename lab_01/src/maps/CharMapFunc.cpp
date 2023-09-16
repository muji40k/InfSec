#include "CharMapFunc.h"

CharMapFunc::CharMapFunc(Func direct, Func inverse, size_t size)
    : limit(size), direct(direct), inverse(inverse)
{}

size_t CharMapFunc::size(void) const
{
    return this->limit;
}

size_t CharMapFunc::encode(size_t c)
{
    return this->direct(c);
}

size_t CharMapFunc::decode(size_t c)
{
    return this->inverse(c);
}

