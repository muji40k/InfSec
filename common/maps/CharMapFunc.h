#ifndef _CHARMAPFUNC_H_
#define _CHARMAPFUNC_H_

#include "CharMap.h"

class CharMapFunc : public CharMap
{
    public:
        using Func = size_t (*)(size_t c);

    public:
        CharMapFunc(Func direct, Func inverse, size_t size);
        virtual ~CharMapFunc(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t encode (size_t c) override;
        virtual size_t decode (size_t c) override;

    private:
        const size_t limit;
        Func direct;
        Func inverse;
};

#endif

