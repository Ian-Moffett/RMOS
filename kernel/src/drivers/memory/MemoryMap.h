#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <stdint.h>

typedef struct {
    uint32_t type;
    void* physAddr;
    void* virtAddr;
    uint64_t nPages;
    uint64_t attr;
} memdesc_t;


typedef struct {
    memdesc_t* mMap;
    uint64_t mSize;
    uint64_t mDescriptorSize;
} meminfo_t;

uint64_t getMemSize(memdesc_t* mMap, uint64_t mEntryCount, uint64_t mDescSize);

extern const char* const MSEGMENTS[];

#endif
