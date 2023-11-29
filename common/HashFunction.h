#ifndef _HASHFUNCTION_H_
#define _HASHFUNCTION_H_

#include <string>

class HashFunction
{
    public:
        virtual ~HashFunction(void) = default;

        virtual std::string apply(const std::string &origin) = 0;
};

#endif

