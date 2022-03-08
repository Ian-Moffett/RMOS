#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "IDT.h"
#include "../drivers/FrameBuffer.h"
#include "../util/string.h"


__attribute__((interrupt)) void div0_handler(int_frame_t* frame);


#endif
