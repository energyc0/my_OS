bits 16
org 0x7c00

NEWLINE equ 0Ah
CARRIAGE_RET equ 0Dh
KERNEL_START equ 0x1000

_start:
    cli

    mov [drive_number], dl ;save drive number

    xor ax,ax ; setup segments
    mov ds, ax
    mov ss, ax
    mov es, ax

    mov bp, 0x7c00 ; setup stack
    mov sp, bp

    mov ah, 00h ;set video mode and clear the screen
    mov al, 03h
    int 10h 

    ;load OS sector to es:bx
load_OS_loop:
    cmp BYTE [load_sector_attemps], 0
    jz load_OS_ERROR
    dec BYTE [load_sector_attemps]

    xor ax,ax
    mov es, ax
    mov bx, KERNEL_START

    mov ah, 02h
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [drive_number]
    int 13h
    jc load_OS_loop ;carry flag is set on error

    lgdt [GDT_descriptor] ;go into protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode

load_OS_ERROR:
    mov si, load_OS_err_msg
    call printstr
    jmp $

%include "boot_output.asm"

load_OS_err_msg: db "Failed to load OS sector!", NEWLINE, CARRIAGE_RET, 0
load_sector_attemps: db 3
drive_number: db 0

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
    
    ;mov al, 'Q'
    ;mov ah, 0xf
   ; mov [0xb8000], ax
    jmp KERNEL_START
    
    hlt

times 510 - ($-$$) db 0
dw 0xAA55