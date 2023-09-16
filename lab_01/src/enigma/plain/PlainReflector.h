#ifndef _PLAINREFLECTOR_H_
#define _PLAINREFLECTOR_H_

#include <memory>

#include "CharMap.h"
#include "EnigmaReflector.h"

class PlainReflector : public EnigmaReflector
{
    public:
        PlainReflector(std::shared_ptr<CharMap> map);
        virtual size_t size(void) const override;
        virtual size_t reflect(size_t c) override;

    private:
        std::shared_ptr<CharMap> map;
};

#endif

