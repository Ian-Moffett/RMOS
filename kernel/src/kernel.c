#include "drivers/video/FrameBuffer.h"
#include "drivers/ps2/Keyboard.h"
#include "drivers/memory/GDT.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"


canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};

__attribute__((interrupt)) void test(int_frame_t*) {
    kwrite(&canvas, "AUDWDIWDUIO\n", 0xFFFFFFFF);
    inportb(0x60);
    outportb(0x20, 0x20);
}


void _start(framebuffer_t* lfb, psf1_font_t* font) {
    canvas.lfb = lfb;
    canvas.font = font;

    gdt_install();
    
    set_idt_vector(0x0, div0_handler, TRAP_GATE_FLAG);
    set_idt_vector(0x21, kb_isr, INT_GATE_FLAG);

    idt_install();

    __asm__ __volatile__("sti");

    
    unmask_irq1();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
