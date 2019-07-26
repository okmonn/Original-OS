#�f�t�H���g����
default:
	make img

#�o�C�i���t�@�C������
%.bin : %.asm Makefile
	nasm $*.asm -o $*.bin -l $*.lst

#�I�u�W�F�N�g�t�@�C������
%.o : %.c Makefile
	gcc -c -m32 -fno-pic -nostdlib -o $*.o $*.c
%.o : %.asm Makefile
	nasm -f elf32 -o $*.o $*.asm

#���p�t�H���g�f�[�^�쐬
hankaku.c : hankaku.txt Font.c Makefile
	gcc Font.c -o Font
	./Font

Main.hrb : hrb.ld Main.o Func.o hankaku.o Init.o Draw.o Math.o Makefile
	ld -m elf_i386 -e Main -o Main.hrb -T hrb.ld Main.o Func.o hankaku.o Init.o Draw.o Math.o

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

clear:
	rm *.lst *.bin *.sys *.img *.hrb *.o