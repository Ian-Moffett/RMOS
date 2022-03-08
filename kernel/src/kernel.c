#include "drivers/FrameBuffer.h"

canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void _start(framebuffer_t* lfb, psf1_font_t* font) {
    canvas.lfb = lfb;
    canvas.font = font;

    kwrite(&canvas, "Hello, World! This is the kernel speaking!\n", 0xFFFFFFFF);
    kwrite(&canvas, "RED!\n", 0xFF0000);
    kwrite(&canvas, "GREEN!\n", 0x00FF00);
    kwrite(&canvas, "BLUE!\n", 0x0000FF);

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
