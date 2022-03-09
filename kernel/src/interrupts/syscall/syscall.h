#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_INT 0x80

#include "../IDT.h"
#include "../exceptions.h"


void __dispatch_syscall();
__attribute__((interrupt)) void syscall_isr(int_frame_t* frame);


 #endif
