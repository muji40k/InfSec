#ifndef _LOGIO_H_
#define _LOGIO_H_

#include <memory>
#include <string>

#include "Alphabet.h"
#include "EnigmaIO.h"

class LogIO : public EnigmaIO
{
    public:
        LogIO(const std::string name, std::shared_ptr<EnigmaIO> io,
              std::shared_ptr<Alphabet<char>> alphabet);
        virtual ~LogIO(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t input(size_t c) override;
        virtual size_t output(size_t c) override;

    private:
        const std::string name;
        std::shared_ptr<EnigmaIO> io;
        std::shared_ptr<Alphabet<char>> alphabet;
};

#endif

