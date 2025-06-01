BUILD_DIR=../build/kernel
KERNEL_NAME=kernel
KERNEL=$(BUILD_DIR)/$(KERNEL_NAME).bin

all: $(BUILD_DIR) $(KERNEL)

$(KERNEL): $(BUILD_DIR)/kernel_start.elf $(BUILD_DIR)/main.o
	ld -m elf_i386 -Tkernel.ld $^ -o $(BUILD_DIR)/$(KERNEL_NAME).out
	objcopy -O binary $(BUILD_DIR)/$(KERNEL_NAME).out $(BUILD_DIR)/$(KERNEL_NAME).bin

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: %.c
	gcc -g -c -m32 -ffreestanding -masm=intel -o $@ $<

$(BUILD_DIR)/%.elf: %.asm
	nasm -f elf32 -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*