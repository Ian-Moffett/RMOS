#ifndef KEYBOARD_H
#define KEYBOARD_H

#define RM_DEBUG

#include "../../interrupts/IDT.h"
#include "../IO/IO.h"
#include "../IO/PIC.h"

#ifdef RM_DEBUG
#include "../video/FrameBuffer.h"
#include "../../util/string.h"
#endif


__attribute__((interrupt)) void kb_isr(int_frame_t* frame);

void unmask_irq1();


#endif
