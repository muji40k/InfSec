#include "PlainReflector.h"

PlainReflector::PlainReflector(std::shared_ptr<CharMap> map)
    : map(map)
{}

size_t PlainReflector::size(void) const
{
    return this->map->size();
}

size_t PlainReflector::reflect(size_t c)
{
    return this->map->encode(c);
}

