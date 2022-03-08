#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../interrupts/IDT.h"
#include "../IO/IO.h"
#include "../IO/PIC.h"

__attribute__((interrupt)) void kb_isr(int_frame_t* frame);

void unmask_irq1();


#endif
