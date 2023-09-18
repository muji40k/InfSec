#include "LogRotor.h"

#include <iostream>
#include <cctype>

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

    char in = this->alphabet->letter(c),
         out = this->alphabet->letter(tmp);

    if (!isprint(in) || isspace(in))
        in = ' ';

    if (!isprint(out) || isspace(out))
        out = ' ';

    std::cout << "[rotor][" << this->name << "][direct]: "
              << "in = " << c << '[' << in << ']' << ", "
              << "out = " << tmp << '[' << out << ']' << std::endl;

    return tmp;
}

size_t LogRotor::invert(size_t c)
{
    size_t tmp = this->map->invert(c);

    char in = this->alphabet->letter(c),
         out = this->alphabet->letter(tmp);

    if (!isprint(in) || isspace(in))
        in = ' ';

    if (!isprint(out) || isspace(out))
        out = ' ';

    std::cout << "[rotor][" << this->name << "][invert]: "
              << "in = " << c << '[' << in << ']' << ", "
              << "out = " << tmp << '[' << out << ']' << std::endl;

    return tmp;
}

