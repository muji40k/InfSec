#ifndef _ENIGMAREFLECTOR_H_
#define _ENIGMAREFLECTOR_H_

#include <cstddef>

class EnigmaReflector
{
    public:
        virtual ~EnigmaReflector(void) = default;
        virtual size_t size(void) const = 0;
        virtual size_t reflect(size_t c) = 0;
};

#endif

