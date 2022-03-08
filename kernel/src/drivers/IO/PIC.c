#include "PIC.h"


void pic_send_EOI(unsigned char irq) {
    if (irq >= 8) {
        outportb(PIC2_COMMAND, PIC_EOI);
    }

    outportb(PIC1_COMMAND, PIC_EOI);
}
