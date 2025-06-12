bits 32

extern kmain
extern _stack_top_
extern _stack_end_
global _kernel_start_

_kernel_start_:
mov bp, _stack_end_
mov sp, bp ; setup stack

call kmain
_kernel_start_loop:
hlt
jmp _kernel_start_loop