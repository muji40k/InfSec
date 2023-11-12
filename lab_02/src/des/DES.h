#ifndef _DES_H_
#define _DES_H_

#include <memory>
#include <vector>
#include <array>

#include "Encoder.h"

#include "BitRange.h"

class SBlock
{
    public:
        virtual ~SBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
};

class FPBlock
{
    public:
        virtual ~FPBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
};

class EBlock
{
    public:
        virtual ~EBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
};

class FBlock
{
    public:
        FBlock(std::shared_ptr<EBlock> e_block,
               std::vector<std::shared_ptr<SBlock>> s_blocks,
               std::shared_ptr<FPBlock> p_block);
        void apply(BitRange in, BitRange key, BitRange out);

    private:
        std::shared_ptr<EBlock> e_block;
        std::vector<std::shared_ptr<SBlock>> s_blocks;
        std::shared_ptr<FPBlock> p_block;
};

class PKeyBlock
{
    public:
        virtual ~PKeyBlock(void) = default;
        virtual void direct(BitRange in, BitRange c, BitRange d) = 0;
};

class HKeyBlock
{
    public:
        virtual ~HKeyBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
};

class ShiftKeyBlock
{
    public:
        virtual ~ShiftKeyBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
};

class KeyBlock
{
    public:
        KeyBlock(BitRange key, std::shared_ptr<PKeyBlock> p_block,
                 std::vector<std::shared_ptr<ShiftKeyBlock>> shift_blocks,
                 std::shared_ptr<HKeyBlock> h_block);
        void get(size_t i, BitRange out);

    private:
        bool check(BitRange key, BitRange out);
        void init(BitRange key);
        void getKey(BitRange in, BitRange out);

    private:
        std::shared_ptr<PKeyBlock>                  p_block;
        std::vector<std::shared_ptr<ShiftKeyBlock>> shift_blocks;
        std::shared_ptr<HKeyBlock>                  h_block;
        std::array<char[6], 16>                     keys;
};

class IPBlock
{
    public:
        virtual ~IPBlock(void) = default;
        virtual void direct(BitRange in, BitRange out) = 0;
        virtual void reverse(BitRange in, BitRange out) = 0;
};

class DES : public Encoder
{
    public:
        DES(std::shared_ptr<IPBlock> ip_block,
            std::shared_ptr<FBlock> f_block,
            std::shared_ptr<KeyBlock> key_block);
        virtual ~DES(void) override = default;
        virtual std::string encode(const std::string &origin) override;
        virtual std::string decode(const std::string &origin) override;

    private:
        void encodeBlock(BitRange current);
        void decodeBlock(BitRange current);

    private:
        std::shared_ptr<IPBlock>  ip_block;
        std::shared_ptr<FBlock>   f_block;
        std::shared_ptr<KeyBlock> key_block;
};

#endif

