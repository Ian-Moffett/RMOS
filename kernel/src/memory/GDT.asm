bits 64

global load_gdt
load_gdt:
    cli
    lgdt [rdi]
    mov rax, 0x0
    mov ds, rax
    mov es, rax
    mov fs, rax
    mov gs, rax
    mov ss, rax
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq
