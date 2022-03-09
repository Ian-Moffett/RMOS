#include "drivers/video/FrameBuffer.h"
#include "drivers/ps2/Keyboard.h"
#include "drivers/memory/GDT.h"
#include "drivers/memory/MemoryMap.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"


canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void _start(framebuffer_t* lfb, psf1_font_t* font, meminfo_t meminfo) {
    canvas.lfb = lfb;
    canvas.font = font;

    gdt_install();

    set_idt_vector(0x0, div0_handler, TRAP_GATE_FLAG);
    set_idt_vector(0xD, gpf_handler, TRAP_GATE_FLAG);
    set_idt_vector(0xE, page_fault_handler, TRAP_GATE_FLAG);
    set_idt_vector(0x21, kb_isr, INT_GATE_FLAG);

    idt_install();

    uint64_t mMapEntries = meminfo.mSize / meminfo.mDescriptorSize;
    kwrite(&canvas, "AVAILABLE MEMORY: ", 0xFFFFFFFF);
    kwrite(&canvas, hex2str(getMemSize(meminfo.mMap, mMapEntries, meminfo.mDescriptorSize)), 0xFFFFFF);
    kwrite(&canvas, " BYTES\n", 0xFFFFFFFF);

    __asm__ __volatile__("sti");
    
    unmask_irq1();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
