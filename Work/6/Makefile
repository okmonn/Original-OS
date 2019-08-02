#アセンブリファイルコンパイル方法
%.bin : %.asm makefile
	nasm $*.asm -o $*.bin -l $*.lst

#Cソースファイルコンパイル方法
%.o : %.c makefile
	gcc -c -m32 -fno-pic -o $*.o $*.c

#C++ソースファイルコンパイル方法
%.o : %.cpp makefile
	g++ -Wall -std=c++17 -c -m32 -fno-pic -fno-stack-protector -o $*.o $*.cpp

#アセンブリファイルをバイナリ出力する
%.o : %.asm makefile
	nasm -f elf32 -o $*.o $*.asm

#hrbファイル作成
bootpack.hrb : har.ld bootpack.o Func.o GateDescriptor.o SegmentDescriptor.o nasmfunc.o makefile
	ld -m elf_i386 -e main -o bootpack.hrb -T har.ld bootpack.o Func.o GateDescriptor.o SegmentDescriptor.o nasmfunc.o

#システムファイル作成
nasmhead.sys : nasmhead.bin bootpack.hrb makefile
	cat nasmhead.bin bootpack.hrb > nasmhead.sys

#イメージファイル作成
helloos.img : ipl10.bin nasmhead.sys makefile
	mformat -f 1440 -C -B ipl10.bin -i helloos.img ::
	mcopy nasmhead.sys -i helloos.img ::

#ファイル削除
clear:
	rm *.bin *.lst *.o *.hrb *.sys *.img

#仮想OS起動
run:
	make -r helloos.img
	qemu-system-i386 -drive file=helloos.img,format=raw,if=floppy
	make clear
