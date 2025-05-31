BUILD_DIR=../build/boot
BOOTLOADER=boot

$(BUILD_DIR)/$(BOOTLOADER).bin: $(BUILD_DIR) $(BOOTLOADER).asm

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.bin: %.asm
	nasm -f bin -o $@  $<


clean:
	rm -f $(BUILD_DIR)/*
