HelloOS.bin : HelloOS.asm Makefile
	nasm HelloOS.asm -o HelloOS.bin -l HelloOS.lst

HelloOS.img : HelloOS.bin Makefile
	cat HelloOS.bin > HelloOS.img
asm:
	make -r HelloOS.bin

img:
	make -r HelloOS.img

run:
	make img
	qemu-system-i386 -drive file=HelloOS.img,format=raw,if=floppy
