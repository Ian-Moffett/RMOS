#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

void heap_init(void* begin, size_t limit);
void* kmalloc(size_t blksize);
void* kfree(void* blk);
uint64_t get_mem_alloc();


#endif
