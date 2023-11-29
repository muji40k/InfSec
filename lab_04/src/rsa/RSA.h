#ifndef _RSA_H_
#define _RSA_H_

#include "Encoder.h"

class RSA : public Encoder
{
    public:
        RSA(unsigned long long n);
        void setPublicKey(unsigned long long e);
        void setPrivateKey(unsigned long long d);

        virtual ~RSA(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        unsigned long long e;
        unsigned long long d;
        const unsigned long long n;

};

#endif

