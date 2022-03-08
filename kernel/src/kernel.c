#include "drivers/FrameBuffer.h"

canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void _start(framebuffer_t* lfb, psf1_font_t* font) {
    canvas.lfb = lfb;
    canvas.font = font;
}
