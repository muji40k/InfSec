#include "LogRotor.h"

#include <iostream>

LogRotor::LogRotor(const std::string name, std::shared_ptr<EnigmaRotor> map,
                   std::shared_ptr<Alphabet<char>> alphabet)
    : name(name), map(map), alphabet(alphabet)
{
    if (map->size() != alphabet->size())
        throw;
}

size_t LogRotor::size(void) const
{
    return this->map->size();
}

size_t LogRotor::direct(size_t c)
{
    size_t tmp = this->map->direct(c);
    std::cout << "[rotor][" << this->name << "][direct]: in = " << this->alphabet->letter(c) << ", out = " << this->alphabet->letter(tmp) << std::endl;

    return tmp;
}

size_t LogRotor::invert(size_t c)
{
    size_t tmp = this->map->invert(c);
    std::cout << "[rotor][" << this->name << "][invert]: in = " << this->alphabet->letter(c) << ", out = " << this->alphabet->letter(tmp) << std::endl;

    return tmp;
}

