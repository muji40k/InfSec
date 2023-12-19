#ifndef _LZW_H_
#define _LZW_H_

#include "Archiver.h"

class LZW : public Archiver
{
    public:
        virtual ~LZW(void) override = default;
        virtual std::string compress(const std::string &origin) override;
        virtual std::string decompress(const std::string &origin) override;
};

#endif

