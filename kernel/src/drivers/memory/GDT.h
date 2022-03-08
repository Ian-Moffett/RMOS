#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdtr_t;


typedef struct {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1;
    uint8_t flags;
} __attribute__((packed)) gdt_descriptor_t;


typedef struct {
    gdt_descriptor_t null;
    gdt_descriptor_t kernelcode;
    gdt_descriptor_t kerneldata;
    gdt_descriptor_t usercode;
    gdt_descriptor_t userdata;
} __attribute__((packed)) __attribute__((aligned(0x1000))) gdt_t;


void gdt_install();

#endif
