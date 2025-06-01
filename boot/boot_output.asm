bits 16
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