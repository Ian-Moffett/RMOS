#include "../pcspkr.h"



void beeep(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;
    outportb(0x43, 0xB6);       // Command byte.
    outportb(0x42, (uint8_t)divisor);
    io_wait();
    outportb(0x42, (uint8_t)divisor >> 8);

    uint8_t tmp = inportb(0x61);

    if (tmp != (tmp | 3)) {
        outportb(0x61, tmp | 3);
    }
}
