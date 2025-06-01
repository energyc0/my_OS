bits 32

extern _main
extern _stack_top_
global _kernel_start_

_kernel_start_:
mov bp, _stack_top_
mov sp, bp ; setup stack

call _main
hlt