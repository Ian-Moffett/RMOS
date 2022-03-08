#include "../IDT.h"

static idt_desc_t idt[256];
static idtr_t idtr;


void set_idt_vector(unsigned char entry, void* isr, unsigned char flags) {
    uint64_t addr = (uint64_t)isr;
    idt[entry].isr_low = addr & 0xFFFF;
    idt[entry].isr_high = (addr >> 32) & 0xFFFFFFFF;
    idt[entry].isr_mid = (addr >> 16) & 0xFFFF;
    idt[entry].selector = 0x08;
    idt[entry].reserved = 0x0;
    idt[entry].attr = flags;
    idt[entry].ist = 0x0;
}


void idt_install() {
    idtr.limit = (uint16_t)sizeof(idt_desc_t) * 256 - 1;
    idtr.base = (uint64_t)&idt;
    __asm__ __volatile__("lidt %0" : : "memory" (idtr));
}
