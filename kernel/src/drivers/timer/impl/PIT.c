#include "../PIT.h"

static uint64_t div = 150;
static uint64_t totalTicks = 0;


void pitSetDivisor(uint16_t freq) {
    if (freq < 100) freq = 100;
    div = BASE_FREQUENCY / freq;
    outportb(0x43, 0x36);               // Command byte.
    io_wait();
    outportb(0x40, (uint8_t)(div & 0xFF));
    io_wait();
    outportb(0x40, (uint8_t)(div >> 8));
}


void sleep(uint64_t ticks) {
    uint64_t eticks = totalTicks + ticks;

    while (totalTicks < eticks) {
        __asm__ __volatile__("hlt");
    }
}


__attribute__((interrupt)) void pit_isr(int_frame_t* frame) {
    ++totalTicks;
}
