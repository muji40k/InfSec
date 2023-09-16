#include "LogIO.h"

#include <iostream>

LogIO::LogIO(const std::string name, std::shared_ptr<EnigmaIO> io,
             std::shared_ptr<Alphabet<char>> alphabet)
    : name(name), io(io), alphabet(alphabet)
{
    if (io->size() != alphabet->size())
        throw;
}

size_t LogIO::size(void) const
{
    return this->io->size();
}

size_t LogIO::input(size_t c)
{
    size_t tmp = this->io->input(c);
    std::cout << "[io][" << this->name << "][input]: in = " << this->alphabet->letter(c) << ", out = " << this->alphabet->letter(tmp) << std::endl;

    return tmp;
}

size_t LogIO::output(size_t c)
{
    size_t tmp = this->io->output(c);
    std::cout << "[io][" << this->name << "][output]: in = " << this->alphabet->letter(c) << ", out = " << this->alphabet->letter(tmp) << std::endl;

    return tmp;
}

