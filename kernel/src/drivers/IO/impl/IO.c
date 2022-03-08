#include "../IO.h"


void outportb(uint16_t port, uint8_t byte) {
    __asm__ __volatile__("out %%al, %%dx" : : "a" (byte), "d" (port));
}


uint8_t inportb(uint16_t port) {
    uint8_t res;
    __asm__ __volatile__("in %%dx, %%al" : "=a" (res));
    return res;
}

void io_wait() {
    __asm__ __volatile__("outb %%al, $0x80" : : "a" (0));
}
