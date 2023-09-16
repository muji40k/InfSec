#include "Enigma.h"

#include <ctype.h>

Enigma::Enigma(std::shared_ptr<Alphabet<char>> alpahbet,
               std::shared_ptr<EnigmaIO> io,
               std::list<std::shared_ptr<EnigmaRotor>> rotors,
               std::shared_ptr<EnigmaReflector> reflector)
    : alpahbet(alpahbet), io(io), rotors(rotors), reflector(reflector)
{
    if (io->size() != alpahbet->size() || reflector->size() != alpahbet->size())
        throw;

    for (auto ptr : rotors)
        if (alpahbet->size() != ptr->size())
            throw;
}

std::string Enigma::encode(const std::string &origin)
{
    return this->process(origin);
}

std::string Enigma::decode(const std::string &origin)
{
    return this->process(origin);
}

std::string Enigma::process(const std::string &origin)
{
    std::string out;

    for (char c : origin)
    {
        size_t i = this->alpahbet->order(c);
        i = this->io->input(i);
        auto iter = this->rotors.begin();

        for (; this->rotors.end() != iter; ++iter)
            i = (*iter)->direct(i);

        i = this->reflector->reflect(i);

        while (this->rotors.begin() != --iter)
            i = (*iter)->invert(i);

        i = (*iter)->invert(i);

        out += this->alpahbet->letter(this->io->output(i));
    }

    return out;
}


