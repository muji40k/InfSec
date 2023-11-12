#ifndef _DESBLOCKS_H_
#define _DESBLOCKS_H_

#include <array>
#include <iostream>

#include "DES.h"
#include "BitPermutator.h"

class BasicSBlock : public SBlock
{
    public:
        BasicSBlock(std::array<std::array<char, 16>, 4> table);
        virtual ~BasicSBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;

    private:
        const std::array<std::array<char, 16>, 4> table;
};

class BasicFPBlock : public FPBlock
{
    public:
        BasicFPBlock(std::shared_ptr<BitPermutator> permutator);
        virtual ~BasicFPBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;

    private:
        std::shared_ptr<BitPermutator> permutator;
};

class BasicEBlock : public EBlock
{
    public:
        virtual ~BasicEBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;
};

class BasicPKeyBlock : public PKeyBlock
{
    public:
        BasicPKeyBlock(std::shared_ptr<BitPermutator> c_perm,
                       std::shared_ptr<BitPermutator> d_perm);
        virtual ~BasicPKeyBlock(void) override = default;
        virtual void direct(BitRange in, BitRange c, BitRange d) override;

    private:
        std::shared_ptr<BitPermutator> c_perm;
        std::shared_ptr<BitPermutator> d_perm;
};

class BasicHKeyBlock : public HKeyBlock
{
    public:
        BasicHKeyBlock(std::shared_ptr<BitPermutator> permutator);
        virtual ~BasicHKeyBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;

    private:
        std::shared_ptr<BitPermutator> permutator;
};

class BasicShiftKeyBlock : public ShiftKeyBlock
{
    public:
        BasicShiftKeyBlock(std::shared_ptr<BitShifter> shifter);
        virtual ~BasicShiftKeyBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;

    private:
        std::shared_ptr<BitShifter> shifter;
};

class BasicIPBlock : public IPBlock
{
    public:
        BasicIPBlock(std::shared_ptr<BitPermutator> permutator);
        virtual ~BasicIPBlock(void) override = default;
        virtual void direct(BitRange in, BitRange out) override;
        virtual void reverse(BitRange in, BitRange out) override;

    private:
        std::shared_ptr<BitPermutator> permutator;
};

#endif

