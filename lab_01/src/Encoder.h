#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <string>

class Encoder
{
    public:
        virtual ~Encoder(void) = default;
        virtual std::string encode(const std::string &origin) = 0;
        virtual std::string decode(const std::string &origin) = 0;
};

#endif

