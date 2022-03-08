#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include "../MemoryMap.h"

typedef struct {
    uint8_t* buffer;
    size_t size;
} bitmap_t;

unsigned char bmpidx(uint64_t index, bitmap_t* bmp);
void readMemMap(memdesc_t* mMap, size_t mMapSize, size_t mDescSize);
void lockPage(void* addr);
void lockPages(void* addr, uint64_t nPages);
void allocPage(void* addr);
void allocPages(void* addr, uint64_t nPages);

#endif
