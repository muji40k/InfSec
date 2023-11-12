#ifndef _CHARMAP_H_
#define _CHARMAP_H_

#include <cstddef>

class CharMap
{
    public:
        virtual ~CharMap(void) = default;
        virtual size_t size(void) const = 0;
        virtual size_t encode (size_t c) = 0;
        virtual size_t decode (size_t c) = 0;
};

#endif

