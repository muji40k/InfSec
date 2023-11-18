#ifndef _CFB_H_
#define _CFB_H_

#include <memory>
#include <list>
#include <vector>

#include <iostream>
#include <iomanip>

#include "Encoder.h"

class CFB : public Encoder
{
    public:
        CFB(std::list<std::shared_ptr<Encoder>> encoders,
            std::vector<size_t> offsets, std::string initial_value,
            const size_t block_size);
        virtual ~CFB(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        std::list<std::shared_ptr<Encoder>> encoders;
        const std::vector<size_t> offsets;
        const std::string initial_value;
        const size_t block_size;
};

#endif

