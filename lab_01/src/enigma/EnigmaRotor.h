#ifndef _ENIGMAROTOR_H_
#define _ENIGMAROTOR_H_

#include <cstddef>

class EnigmaRotor
{
    public:
        virtual ~EnigmaRotor(void) = default;
        virtual size_t size(void) const = 0;
        virtual size_t direct(size_t c) = 0;
        virtual size_t invert(size_t c) = 0;
};

#endif

