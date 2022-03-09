#include "../Keyboard.h"

char keyBuf;
unsigned char keyBufFull = 0;
extern canvas_t canvas;

static const uint8_t* const SC_ASCII = "\x00\x1B" "1234567890-=" "\x08"
"\x00" "qwertyuiop[]" "\x0D\x1D" "asdfghjkl;'`" "\x00" "\\"
"zxcvbnm,./" "\x00\x00\x00" " ";

__attribute__((interrupt)) void kb_isr(int_frame_t* frame) {
    unsigned int scancode = inportb(0x60);

    if (!(scancode & 0x80)) {
        char c = SC_ASCII[scancode];

        if (c >= 'a' && c <= 'z') {
            c -= 0x20;
        }

        keyBuf = c;
        keyBufFull = 1;
    }

    pic_send_EOI(1);

}


void unmask_irq1() {
    /*
     *  11111111
     *
     *  00000001 << 1
     *  00000010
     *
     *  11111111
     *  00000010
     */

    outportb(PIC1_DATA, inportb(PIC1_DATA) ^ (1 << 1)); 
}
