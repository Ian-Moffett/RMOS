bits 64
global __dispatch_syscall

%define SYSCALL_TABLE_SIZE 1
extern syscall_table

__dispatch_syscall:
    cmp rax, SYSCALL_TABLE_SIZE
    jge .dispatch_failure
    cmp rax, 0
    jl .dispatch_failure

    mov rdx, [syscall_table]
    add rdx, rax
    call rdx
    iret

    .dispatch_failure:
        iret
