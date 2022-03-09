bits 64
global __gpf

__gpf:
    mov rax, 0x0
    int 0x80
    ret             ; RET ON FAILURE.
