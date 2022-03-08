#ifndef PIC_H
#define PIC_H

#define PIC1_COMMAND 0x20       // Master.
#define PIC1_DATA 0x21          // Master data line.
#define PIC2_COMMAND 0xA0       // Slave chip.
#define PIC2_DATA   0xA1        // Slave data.

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
#define PIC_EOI 0x20

#include "IO.h"

void pic_send_EOI(unsigned char irq);


#endif
