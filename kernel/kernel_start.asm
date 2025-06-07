bits 32

extern kmain
extern _stack_top_
global _kernel_start_

_kernel_start_:
mov bp, _stack_top_
mov sp, bp ; setup stack

call kmain
_kernel_start_loop:
hlt
jmp _kernel_start_loop