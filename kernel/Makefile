BUILD_DIR=../build/kernel
KERNEL_NAME=kernel
KERNEL=$(BUILD_DIR)/$(KERNEL_NAME).bin
CFLAGS=-ggdb3 -c -m32 -ffreestanding -masm=intel -O2 -Wall -Wextra -fno-pic -nostdlib

SRC_C=$(wildcard *.c)
SRC_ASM=$(wildcard *.asm)

SRC=$(SRC_C) $(SRC_ASM)
OBJ_FILES=$(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_C)) $(patsubst %.asm, $(BUILD_DIR)/%.elf, $(SRC_ASM))


all: $(BUILD_DIR) $(KERNEL)

$(KERNEL): $(OBJ_FILES)
	ld -z noexecstack -no-pie -nmagic -m elf_i386 -Tkernel.ld $^ -o $(BUILD_DIR)/$(KERNEL_NAME).out
	objcopy -O binary $(BUILD_DIR)/$(KERNEL_NAME).out $(BUILD_DIR)/$(KERNEL_NAME).bin
	objcopy --only-keep-debug $(BUILD_DIR)/$(KERNEL_NAME).out $(BUILD_DIR)/$(KERNEL_NAME).dbg

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: %.c
	gcc $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.elf: %.asm
	nasm -f elf32 -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*