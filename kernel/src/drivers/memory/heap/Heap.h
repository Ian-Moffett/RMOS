#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>
#include "../../../util/string.h"

void heap_init(void* begin, size_t limit);
void* kmalloc(size_t blksize);
void* kfree(void* blk);
void* krealloc(void* blk, size_t blksize);
uint64_t get_mem_alloc();


#endif
