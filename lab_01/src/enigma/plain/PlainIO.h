#ifndef _PLAINIO_H_
#define _PLAINIO_H_

#include <memory>

#include "CharMap.h"
#include "EnigmaIO.h"

class PlainIO : public EnigmaIO
{
    public:
        PlainIO(std::shared_ptr<CharMap> map);
        virtual size_t size(void) const override;
        virtual size_t input(size_t c) override;
        virtual size_t output(size_t c) override;

    private:
        std::shared_ptr<CharMap> map;
};

#endif

