#include "drivers/FrameBuffer.h"
#include "memory/GDT.h"

canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void _start(framebuffer_t* lfb, psf1_font_t* font) {
    canvas.lfb = lfb;
    canvas.font = font;

    gdt_install();

    // if (0 / 0 == 55) {}

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
