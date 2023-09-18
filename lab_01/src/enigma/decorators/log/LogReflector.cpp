#include "LogReflector.h"

#include <iostream>
#include <cctype>

LogReflector::LogReflector(const std::string name, std::shared_ptr<EnigmaReflector> ref,
                           std::shared_ptr<Alphabet<char>> alphabet)
    : name(name), ref(ref), alphabet(alphabet)
{
    if (ref->size() != alphabet->size())
        throw;
}

size_t LogReflector::size(void) const
{
    return this->ref->size();
}

size_t LogReflector::reflect(size_t c)
{
    size_t tmp = this->ref->reflect(c);

    char in = this->alphabet->letter(c),
         out = this->alphabet->letter(tmp);

    if (!isprint(in) || isspace(in))
        in = ' ';

    if (!isprint(out) || isspace(out))
        out = ' ';

    std::cout << "[reflector][" << this->name << "][reflect]: "
              << "in = " << c << '[' << in << ']' << ", "
              << "out = " << tmp << '[' << out << ']' << std::endl;

    return tmp;
}


