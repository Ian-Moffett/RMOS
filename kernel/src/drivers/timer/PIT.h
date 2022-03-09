#ifndef PIT_H
#define PIT_H

#define BASE_FREQUENCY 1193180

#include "../IO/IO.h"
#include "../../interrupts/IDT.h"


void pitSetDivisor(uint16_t freq);
void sleep(uint64_t ticks);
__attribute__((interrupt)) void pit_isr(int_frame_t* frame);


#endif
