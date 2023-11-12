#ifndef _CHARALPHABET_H_
#define _CHARALPHABET_H_

#include "Alphabet.h"

class CharAlphabet : public Alphabet<char>
{
    public:
        virtual ~CharAlphabet(void) override = default;
        virtual size_t size(void) const override;
        virtual char letter(size_t c) override;
        virtual size_t order(char c) override;
};

#endif

