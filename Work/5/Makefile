default:
	make img

hankaku.c : hankaku.txt Font.c Makefile
	gcc Font.c -o Font
	./Font

hankaku.o : hankaku.c Makefile
	gcc -c -m32 -nostdlib -o hankaku.o hankaku.c

HelloOS.bin : HelloOS.asm Makefile
	nasm HelloOS.asm -o HelloOS.bin -l HelloOS.lst

okmonn.bin : okmonn.asm Makefile
	nasm okmonn.asm -o okmonn.bin -l okmonn.lst

Func.o : Func.asm Makefile
	nasm -f elf32 -o Func.o Func.asm

Main.o : Main.c Makefile
	gcc -c -m32 -fno-pic -o Main.o Main.c

Main.hrb : Main.o Func.o hankaku.o hrb.ld Makefile
	ld -m elf_i386 -e Main -o Main.hrb -T hrb.ld Main.o Func.o hankaku.o

okmonn.sys : okmonn.bin Main.hrb Makefile
	cat okmonn.bin Main.hrb > okmonn.sys

okmonn.img : okmonn.sys HelloOS.bin Makefile
	mformat -f 1440 -C -B HelloOS.bin -i okmonn.img ::
	mcopy okmonn.sys -i okmonn.img ::

asm:
	make -r HelloOS.bin

img:
	make -r okmonn.img

run:
	make img
	qemu-system-i386 -drive file=okmonn.img,format=raw,if=floppy

clean:
	rm *.lst *.bin *.sys *.img *.hrb *.o
