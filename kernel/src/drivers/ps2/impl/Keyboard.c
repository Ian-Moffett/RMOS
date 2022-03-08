#include "../Keyboard.h"

char keyBuf;


__attribute__((interrupt)) void kb_isr(int_frame_t* frame) {
    unsigned int scancode = inportb(0x60);
    pic_send_EOI(1);
}


void unmask_irq1() {
    /*
     *  11111111
     *
     *  00000001 << 1
     *  00000010
     *
     *  11111111
     *  00000010
     */

    outportb(PIC1_DATA, inportb(PIC1_DATA) ^ (1 << 1)); 
}
