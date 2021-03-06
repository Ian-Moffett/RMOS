#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "IDT.h"
#include "../drivers/video/FrameBuffer.h"
#include "../drivers/audio/pcspkr.h"
#include "../util/string.h"


__attribute__((interrupt)) void div0_handler(int_frame_t* frame);
__attribute__((interrupt)) void gpf_handler(int_frame_t* frame);
__attribute__((interrupt)) void page_fault_handler(int_frame_t* frame);


#endif
