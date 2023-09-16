#include "PlainRotor.h"

PlainRotor::PlainRotor(std::shared_ptr<CharMap> map)
    : map(map)
{}

size_t PlainRotor::size(void) const
{
    return this->map->size();
}

size_t PlainRotor::direct(size_t c)
{
    return this->map->encode(c);
}

size_t PlainRotor::invert(size_t c)
{
    return this->map->decode(c);
}

