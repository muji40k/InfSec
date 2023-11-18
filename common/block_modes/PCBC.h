#ifndef _PCBC_H_
#define _PCBC_H_

#include <memory>
#include <list>

#include <iostream>
#include <iomanip>

#include "Encoder.h"

class PCBC : public Encoder
{
    public:
        PCBC(std::list<std::shared_ptr<Encoder>> encoders,
             std::string initial_value, const size_t block_size);
        virtual ~PCBC(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        std::list<std::shared_ptr<Encoder>> encoders;
        const std::string initial_value;
        const size_t block_size;
};

#endif

