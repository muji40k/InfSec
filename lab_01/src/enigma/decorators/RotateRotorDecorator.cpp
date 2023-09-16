#include "RotateRotorDecorator.h"

RotateRotorDecorator::RotateRotorDecorator(std::shared_ptr<EnigmaRotor> rotor)
    : rotor(rotor)
{}

size_t RotateRotorDecorator::size(void) const
{
    return this->rotor->size();
}

size_t RotateRotorDecorator::direct(size_t c)
{
    if (!this->isPrevious)
        this->adjust();

    size_t tmp = this->apply_correction(c);
    tmp = this->rotor->direct(tmp);

    return this->undo_correction(tmp);
}

size_t RotateRotorDecorator::invert(size_t c)
{
    size_t tmp = this->apply_correction(c);
    tmp = this->rotor->invert(tmp);

    return this->undo_correction(tmp);
}

void RotateRotorDecorator::stack(std::shared_ptr<RotateRotorDecorator> previous)
{
    previous->registerFullTurn(this);
    this->isPrevious = true;
}

void RotateRotorDecorator::registerFullTurn(RotateRotorDecorator *next)
{
    this->next = next;
}

void RotateRotorDecorator::adjust(void)
{
    if (this->rotor->size() <= ++this->correction)
    {
        this->correction = 0;

        if (this->next)
            this->next->adjust();
    }
}

size_t RotateRotorDecorator::apply_correction(size_t c) const
{
    c += this->correction;

    if (c >= this->rotor->size())
        c -= this->rotor->size();

    return c;
}

size_t RotateRotorDecorator::undo_correction(size_t c) const
{
    if (c >= this->correction)
        return  c - this->correction;

    return this->rotor->size() - this->correction + c;
}

