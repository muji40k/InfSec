#ifndef _LOGREFLECTOR_H_
#define _LOGREFLECTOR_H_

#include <memory>
#include <string>

#include "EnigmaReflector.h"
#include "Alphabet.h"

class LogReflector : public EnigmaReflector
{
    public:
        LogReflector(const std::string name, std::shared_ptr<EnigmaReflector> ref,
                     std::shared_ptr<Alphabet<char>> alphabet);
        virtual ~LogReflector(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t reflect(size_t c) override;

    private:
        const std::string name;
        std::shared_ptr<EnigmaReflector> ref;
        std::shared_ptr<Alphabet<char>> alphabet;
};

#endif

