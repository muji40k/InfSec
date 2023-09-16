#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include <memory>
#include <list>

#include "Alphabet.h"
#include "Encoder.h"
#include "EnigmaIO.h"
#include "EnigmaRotor.h"
#include "EnigmaReflector.h"

class Enigma : public Encoder
{
    public:
        Enigma(std::shared_ptr<Alphabet<char>> alpahbet,
               std::shared_ptr<EnigmaIO> io,
               std::list<std::shared_ptr<EnigmaRotor>> rotors,
               std::shared_ptr<EnigmaReflector> reflector);
        virtual ~Enigma(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        std::string process(const std::string &origin);

    private:
        std::shared_ptr<Alphabet<char>> alpahbet;
        std::shared_ptr<EnigmaIO> io;
        std::list<std::shared_ptr<EnigmaRotor>> rotors;
        std::shared_ptr<EnigmaReflector> reflector;
};

#endif

