bits 16
org 0x7c00

NEWLINE equ 0Ah
CARRIAGE_RET equ 0Dh

_start:
    cli

    xor ax,ax ; setup segments
    mov ds, ax
    mov ss, ax
    mov es, ax

    mov bp, 0x7c00 ; setup stack
    mov sp, bp

    mov si, msg
    call printstr
    jmp $

; al = character to write
putchar:
    push ax
    mov ah, 0eh
    int 10h
    pop ax
    ret

; es:si = null-terminated string
printstr:
    push es
    push si
.print_loop:
    lodsb
    cmp al, 0
    jz .print_loop_end
    call putchar
    jmp .print_loop
.print_loop_end:
    pop si
    pop es
    ret


msg: db 'Hello, I am Maxim', NEWLINE, CARRIAGE_RET

times 510 - ($-$$) db 0
dw 0xAA55