bits 32

extern main
global _kernel_start_

_kernel_start_:
call main
hlt