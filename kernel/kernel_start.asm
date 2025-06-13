bits 32

extern kmain
extern _stack_top_
global _kernel_start_

_kernel_start_:
mov ebp, _stack_top_
mov esp, ebp ; setup stack

call kmain
_kernel_start_loop:
hlt
jmp _kernel_start_loop