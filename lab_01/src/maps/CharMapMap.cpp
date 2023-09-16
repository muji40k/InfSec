#include "CharMapMap.h"

CharMapMap::CharMapMap(std::list<std::pair<size_t, size_t>> map)
{
    for (auto item : map)
    {
        this->direct.emplace(item);
        this->inverse.emplace(std::make_pair(item.second, item.first));
    }
}

size_t CharMapMap::size(void) const
{
    return this->direct.size();
}

size_t CharMapMap::encode(size_t c)
{
    return this->direct.at(c);
}

size_t CharMapMap::decode(size_t c)
{
    return this->inverse.at(c);
}
