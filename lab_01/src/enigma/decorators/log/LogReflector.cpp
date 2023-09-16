#include "LogReflector.h"

#include <iostream>

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
    std::cout << "[reflector][" << this->name << "][reflect]: in = " << this->alphabet->letter(c) << ", out = " << this->alphabet->letter(tmp) << std::endl;

    return tmp;
}


