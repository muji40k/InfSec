#ifndef _ROTATEROTORDECORATOR_H_
#define _ROTATEROTORDECORATOR_H_

#include <memory>

#include "EnigmaRotor.h"

class RotateRotorDecorator : public EnigmaRotor
{
    public:
        RotateRotorDecorator(std::shared_ptr<EnigmaRotor> rotor);
        virtual ~RotateRotorDecorator(void) override = default;
        virtual size_t size(void) const override;
        virtual size_t direct(size_t c) override;
        virtual size_t invert(size_t c) override;

        void stack(std::shared_ptr<RotateRotorDecorator> previous);

    private:
        void registerFullTurn(RotateRotorDecorator *next);
        void adjust(void);

        size_t apply_correction(size_t c) const;
        size_t undo_correction(size_t c) const;

    private:
        std::shared_ptr<EnigmaRotor> rotor;
        size_t correction = 0;
        RotateRotorDecorator *next = nullptr;
        bool isPrevious = false;
};

#endif

