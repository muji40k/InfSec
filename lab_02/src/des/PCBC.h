#ifndef _PCBC_H_
#define _PCBC_H_

#include <memory>
#include <list>

#include <iostream>
#include <iomanip>

#include "Encoder.h"
#include "DES.h"

class PCBC : public Encoder
{
    public:
        PCBC(std::list<std::shared_ptr<DES>> des,
             const char *const initial_value);
        virtual ~PCBC(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        std::list<std::shared_ptr<DES>> des;
        char initial_value[8];
};

#endif

