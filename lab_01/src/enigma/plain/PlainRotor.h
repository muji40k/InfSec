#ifndef _PLAINROTOR_H_
#define _PLAINROTOR_H_

#include <memory>

#include "CharMap.h"
#include "EnigmaRotor.h"

class PlainRotor : public EnigmaRotor
{
    public:
        PlainRotor(std::shared_ptr<CharMap> map);
        virtual ~PlainRotor(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t direct(size_t c) override;
        virtual size_t invert(size_t c) override;

    private:
        std::shared_ptr<CharMap> map;
};

#endif

