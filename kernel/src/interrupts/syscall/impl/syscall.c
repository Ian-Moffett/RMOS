#include "../syscall.h"

#define SYSCALL_TABLE_SIZE 1


void syscall_test_fault() {
    // Causes GPF or page fault.
    *(char*)(0xFFFFFFFFFFFFFFF) = 0x0;
}


void* syscall_table[SYSCALL_TABLE_SIZE] = {
        syscall_table,
};
