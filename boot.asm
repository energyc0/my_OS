bits 16
org 0x7c00

NEWLINE equ 0Ah
CARRIAGE_RET equ 0Dh

_start:
    cli

    mov [bootdrive], dl ;save bootdrive

    xor ax,ax ; setup segments
    mov ds, ax
    mov ss, ax
    mov es, ax

    mov bp, 0x7c00 ; setup stack
    mov sp, bp

    ;mov si, msg
    ;call printstr

    ;go into protected mode

    lgdt [GDT_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode

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

; AH = Scan code of the key pressed down, AL = ASCII character of the button pressed
readchar:
    mov ah, 0h
    int 16h
    ret

msg: db 'Hello, I am Maxim', NEWLINE, CARRIAGE_RET
bootdrive: db 0

GDT_descriptor:
    dw GDT_struct_end - GDT_struct_start - 1
    dd GDT_struct_start

GDT_struct_start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff ; limit 16 bits
        dw 0x0 ; base 16 bits
        db 0x0 ; base 8 bits
        db 0b10011110 ; access byte
        db 0b11001111 ; flags + limit 4 bits
        db 0x0 ; base 8 bits
    data_descriptor:
        dw 0xffff ; limit 16 bits
        dw 0x0 ; base 16 bits
        db 0x0 ; base 8 bits
        db 0b10010010 ; access byte
        db 0b11001111 ; flags + limit 4 bits
        db 0x0 ; base 8 bits
GDT_struct_end:

GDT_size equ GDT_struct_end - GDT_struct_start - 1

CODE_SEG equ code_descriptor - GDT_struct_start
DATA_SEG equ data_descriptor - GDT_struct_start

bits 32
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov al, 'Q'
    mov ah, 0x0f
    mov [0xb8000], ax
    hlt

times 510 - ($-$$) db 0
dw 0xAA55