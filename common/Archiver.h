#ifndef _ARCHIVER_H_
#define _ARCHIVER_H_

#include <string>

class Archiver
{
    public:
        virtual ~Archiver(void) = default;
        virtual std::string compress(const std::string &origin) = 0;
        virtual std::string decompress(const std::string &origin) = 0;
};

#endif

