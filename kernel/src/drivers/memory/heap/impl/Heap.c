#include "../Heap.h"

typedef struct MEM_BLOCK {
    struct MEM_BLOCK* next;
    size_t size;
    unsigned char free;
} memblock_t;


#define DATA_START(memblock) ((char*)memblock + sizeof(memblock_t))


static memblock_t* mem_head = NULL;
static memblock_t* mem_tail = NULL;
static size_t limit = 0;
static size_t allocated = 0;


void heap_init(void* begin, size_t limit) {
    mem_head = (memblock_t*)begin;
    mem_head->next = NULL;
    mem_head->size = 0;
    mem_head->free = 0x1;
    mem_tail = mem_head;
    limit = limit;
}

static memblock_t* first_fit(size_t size) {
    memblock_t* cur = mem_head;

    while (cur != NULL) {
        if (!(cur->free)) {
            cur = cur->next;
        } else if (cur->size >= size) {
            return cur;
        }
    }

    return NULL;
}


void* kmalloc(size_t blksize) {
    if (allocated + blksize > limit) {
        return NULL;
    }

    memblock_t* region = first_fit(blksize);

    if (region == NULL) {
        char* next = DATA_START(mem_tail) + mem_tail->size;
        mem_tail->next = (memblock_t*)next;
        region = mem_tail->next;
        region->size = blksize;
        region->free = 0x0;
        region->next = NULL;
        mem_tail = region;
        allocated += blksize;
    }

    return DATA_START(region);
}



void* kfree(void* blk) {
    memblock_t* curFrame = mem_head;

    while (curFrame != NULL && blk != DATA_START(curFrame)) {
        curFrame = curFrame->next;
    }

    curFrame->free = 0x1;
    allocated -= curFrame->size;
}
