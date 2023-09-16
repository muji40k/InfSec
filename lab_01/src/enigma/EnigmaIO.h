#ifndef _ENIGMAIO_H_
#define _ENIGMAIO_H_

#include <cstddef>

class EnigmaIO
{
    public:
        virtual ~EnigmaIO(void) = default;
        virtual size_t size(void) const = 0;
        virtual size_t input(size_t c) = 0;
        virtual size_t output(size_t c) = 0;
};

#endif

