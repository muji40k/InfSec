#ifndef _AES_H_
#define _AES_H_

#include <memory>

#include "Encoder.h"

class KeyBlock;

class AES : public Encoder
{
    public:
        AES(const std::string &key);
        virtual ~AES(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        void encodeBlock(const unsigned char *in, unsigned char *out);
        void decodeBlock(const unsigned char *in, unsigned char *out);

    private:
        std::shared_ptr<KeyBlock> key_block;
        size_t iters;
};

#endif

