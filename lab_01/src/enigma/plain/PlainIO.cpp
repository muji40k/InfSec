#include "PlainIO.h"

PlainIO::PlainIO(std::shared_ptr<CharMap> map)
    : map(map)
{}

size_t PlainIO::size(void) const
{
    return map->size();
}

size_t PlainIO::input(size_t c)
{
    return map->encode(c);
}

size_t PlainIO::output(size_t c)
{
    return map->decode(c);
}

