#include "../paging.h"


static uint64_t freeMem = 0;
static uint64_t reservedMem = 0;
static uint64_t usedMem = 0;
static bitmap_t bitmap;


unsigned char bmpidx(uint64_t index, bitmap_t* bmp) {
    if (bmp->buffer[index / 8] & 0b10000000 >> (index % 8)) {
        return 1;
    }

    return 0;
}

void lockPage(void* addr) {
    uint64_t index = (uint64_t)addr / 4096;

    if (bmpidx(index, &bitmap)) {
        return;
    }

    freeMem -= 4096;
    usedMem += 4096;

    bitmap.buffer[index / 8] |= 0b10000000 >> (index % 8);

}


void lockPages(void* addr, uint64_t nPages) {
    for (int i = 0; i < nPages; ++i) {
        lockPage((void*)((uint64_t)addr + (i * 4096)));
    }
}



void allocPage(void* addr) {
    uint64_t idx = (uint64_t)addr / 4096;
    if (bmpidx(idx, &bitmap)) {
        return;
    }

    bitmap.buffer[idx / 8] |= 0b10000000 >> (idx % 8);
    freeMem -= 4096;
    reservedMem += 4096;
}


void allocPages(void* addr, uint64_t nPages) {
    for (int i = 0; i < nPages; ++i) {
        allocPage((void*)((uint64_t)addr + (i * 4096)));
    }
}


void readMemMap(memdesc_t* mMap, size_t mMapSize, size_t mDescSize) {
    static uint8_t init = 0;

    if (init) {
        return;
    }

    init = 1;

    uint64_t mMapEntries = mMapSize / mDescSize;
    size_t biggestSegSize = 0;
    void* biggestSeg = NULL;

    for (int i = 0; i < mMapEntries; ++i) {
        memdesc_t* desc = (memdesc_t*)((uint64_t)mMap + (i * mDescSize));

        if (desc->type == 7) {      // Conventional Memory.
            if (desc->nPages * 4096 > biggestSegSize) {
                biggestSegSize = desc->nPages * 4096;   // 4KB pages.
                biggestSeg = desc->physAddr;
            }
        }
    }

    uint64_t memSz = getMemSize(mMap, mMapEntries, mDescSize);

    freeMem = memSz;
    uint64_t bmpSize = memSz / 4096 / 8 + 1;

    bitmap.size = bmpSize;
    bitmap.buffer = (uint8_t*)biggestSeg;

    for (int i = 0; i < bitmap.size; ++i) {
        memdesc_t* desc = (memdesc_t*)((uint64_t)mMap + (i * mDescSize));

        if (desc->type != 7) {   // NonConventionalMemory.
            allocPages(desc->physAddr, desc->nPages);
        }
    }

    // Lock pages where bmp is so we cannot access it
    // while requesting pages.

    lockPages(&bitmap, bitmap.size / 4096 + 1);
}
