BUILD_DIR=build
DISK=$(BUILD_DIR)/disk.img

BOOTLOADER_DIR=boot
KERNEL_DIR=kernel

BOOTLOADER=$(BUILD_DIR)/$(BOOTLOADER_DIR)/boot.bin
KERNEL=$(BUILD_DIR)/$(KERNEL_DIR)/kernel.bin

all: $(DISK)

$(DISK): $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOTLOADER) of=$@ bs=512 count=1 seek=0 conv=notrunc
	dd if=$(KERNEL) of=$@ bs=512 count=$$(($(shell stat $(KERNEL) --printf="%s")/512)) seek=1 conv=notrunc

$(BOOTLOADER): 
	make -C $(BOOTLOADER_DIR)

$(KERNEL): 
	make -C $(KERNEL_DIR)

clean:
	rm -f $(DISK)
	make clean -C $(KERNEL_DIR)
	make clean -C $(BOOTLOADER_DIR)

qemu:
	qemu-system-i386 -S -gdb tcp::1234 -fda $(DISK) &
	clear
	gdb

.PHONY: $(BOOTLOADER) $(KERNEL) $(DISK)