#ifndef _MD5_H_
#define _MD5_H_

#include "HashFunction.h"

class MD5 : public HashFunction
{
    public:
        virtual ~MD5(void) override = default;

        virtual std::string apply(const std::string &origin) override;

    private:
        using FFunc = unsigned int (*)(const unsigned int, const unsigned int, const unsigned int);
        using Iter = struct
        {
            FFunc func;
            size_t k;
        };

        static void getIteration(const size_t i, Iter &handler);

        static unsigned int F(const unsigned int b, const unsigned int c, const unsigned int d);
        static unsigned int G(const unsigned int b, const unsigned int c, const unsigned int d);
        static unsigned int H(const unsigned int b, const unsigned int c, const unsigned int d);
        static unsigned int I(const unsigned int b, const unsigned int c, const unsigned int d);

    private:
        static const unsigned char expansion[64];
        static const size_t S[64];
        static const unsigned int K[64];
};

#endif

