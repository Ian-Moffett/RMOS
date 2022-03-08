#include "../exceptions.h"


static void crapout(unsigned char vector, int_frame_t* frame) {
    extern canvas_t canvas;
    clearScreen(&canvas, 0xFF0000);
    kwrite(&canvas, "FATAL CPU EXCEPTION: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(vector), 0xFFFFFFFF);

    if (vector == 0x0) {
        kwrite(&canvas, "\n\nDUMBASS_DETECTED, YOU DID A STUPID! (also, don't divide by zero lmaoo)\n\n", 0xFFFFFFFF);
    }

    kwrite(&canvas, "\n<==== INTERRUPT FRAME DUMP ====>\n\n", 0xFFFFFFFF);
    kwrite(&canvas, "RIP: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(frame->rip), 0xFFFFFFFF);
    kwrite(&canvas, "\nCS: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(frame->cs), 0xFFFFFFFF);
    kwrite(&canvas, "\nRFLAGS: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(frame->rflags), 0xFFFFFFFF);
    kwrite(&canvas, "\nRSP: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(frame->rsp), 0xFFFFFFFF);
    kwrite(&canvas, "\nSS: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(frame->ss), 0xFFFFFFFF);

    kwrite(&canvas, "\n\n<==== PHYSICAL MEMORY DUMP ====>\n\n", 0xFFFFFFFF);

    uint64_t* start = (uint64_t*)frame->rsp;
    unsigned int offset = 0;

    for (uint64_t* addr = start; addr > start - 50; --addr) {
        kwrite(&canvas, hex2str(*addr), 0xFFFFFFFF);
        kwrite(&canvas, " ", 0xFFFFFFFF);
        ++offset;

        if (offset > 10) {
            offset = 0;
            kwrite(&canvas, "\n", 0xFFFFFFFF);
        }
    }

    __asm__ __volatile__("cli; hlt");
}


__attribute__((interrupt)) void div0_handler(int_frame_t* frame) {
    beeep(48);             // Lmaoooo.
    crapout(0x0, frame);
}
