#ifndef _CHARMAPMAP_H_
#define _CHARMAPMAP_H_

#include <unordered_map>
#include <list>
#include <utility>

#include "CharMap.h"

class CharMapMap : public CharMap
{
    public:
        CharMapMap(std::list<std::pair<size_t, size_t>> map);
        virtual ~CharMapMap(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t encode(size_t c) override;
        virtual size_t decode(size_t c) override;

    private:
        std::unordered_map<size_t, size_t> direct;
        std::unordered_map<size_t, size_t> inverse;
};

#endif

