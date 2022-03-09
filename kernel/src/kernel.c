#include "drivers/video/FrameBuffer.h"
#include "drivers/ps2/Keyboard.h"
#include "drivers/memory/GDT.h"
#include "drivers/memory/MemoryMap.h"
#include "drivers/memory/heap/Heap.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"


canvas_t canvas = {
    .x = 10,
    .y = 10,
    .prevX = 10,
};


void* heap = NULL;


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

    uint64_t mMapEntryCount = meminfo.mSize / meminfo.mDescriptorSize;

    for (int i = 0; i < mMapEntryCount; ++i) {
        memdesc_t* desc = (memdesc_t*)((uint64_t)meminfo.mMap + (i * meminfo.mDescriptorSize));

        if (desc->type == 7) {
            // EfiConventionalMemory
            heap_init(desc->physAddr, desc->nPages * 4096 - 1);
            heap = desc->physAddr;
            kwrite(&canvas, "HEAP SEGMENT ALLOCATED!\n", 0x000000FF);
            break;
        }
    }

    unmask_irq1();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
