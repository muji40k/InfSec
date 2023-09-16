#ifndef _LOGROTOR_H_
#define _LOGROTOR_H_

#include <memory>
#include <string>

#include "EnigmaRotor.h"
#include "Alphabet.h"

class LogRotor : public EnigmaRotor
{
    public:
        LogRotor(const std::string name, std::shared_ptr<EnigmaRotor> map,
                 std::shared_ptr<Alphabet<char>> alphabet);
        virtual ~LogRotor(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t direct(size_t c) override;
        virtual size_t invert(size_t c) override;

    private:
        const std::string name;
        std::shared_ptr<EnigmaRotor> map;
        std::shared_ptr<Alphabet<char>> alphabet;
};

#endif

