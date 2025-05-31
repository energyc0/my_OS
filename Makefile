DISK=disk.img

all: $(DISK)

$(DISK): boot.o
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=boot.o of=$@ bs=512 count=1 seek=0 conv=notrunc

%.o: %.asm
	nasm -f bin -o $@  $<

clean:
	rm *.o $(DISK)

qemu:
	qemu-system-i386 -S -gdb tcp::1234 -fda $(DISK) &