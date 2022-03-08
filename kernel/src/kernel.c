#include "drivers/FrameBuffer.h"
#include "drivers/Keyboard.h"
#include "memory/GDT.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"


canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void _start(framebuffer_t* lfb, psf1_font_t* font) {
    canvas.lfb = lfb;
    canvas.font = font;

    gdt_install();
    
    set_idt_vector(0x0, div0_handler, TRAP_GATE_FLAG);
    set_idt_vector(0x21, div0_handler, INT_GATE_FLAG);

    __asm__ __volatile__("sti");

    idt_install();
    unmask_irq1();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
