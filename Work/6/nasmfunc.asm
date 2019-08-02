; ä÷êîêÈåæ
section .text
	global io_hlt, io_cli, io_sti, io_stihls
	global io_out8
	global io_in8
	global io_store_eflags, io_load_eflags
	global load_gdt, load_idt
	global inithandle21, inithandle2c
	extern InitHandle21, InitHandle2c
	global write_mem8

; void io_hlt(void)
io_hlt:
	HLT
	RET

; void io_cli(void)
io_cli:
	CLI;
	RET

; void io_sti(void)
io_sti:
	STI
	RET

; void io_stihlt(void)
io_stihlt:
	STI
	HLT
	RET

; void io_out8(int port, int data)
io_out8:
	MOV	EDX, [ESP + 4]	; port
	MOV	EAX, [ESP + 8]	; data
	OUT DX, AX
	RET

; int io_in8(int port)
io_in8:
	MOV	EDX, [ESP + 4]	; port
	MOV	EAX, 0
	IN	AL, DX
	RET

; void io_store_eflags(int eflags)
io_store_eflags:
	MOV		EAX, [ESP + 4]
	PUSH	EAX
	POPFD					; POP EFLAGS Ç∆Ç¢Ç§à”ñ°
	RET

; int io_load_eflags(void)
io_load_eflags:
	PUSHFD	; PUSH EFLAGS Ç∆Ç¢Ç§à”ñ°
	POP	EAX
	RET

; void load_gdt(int limit, int addr)
load_gdt:
	MOV		AX, [ESP + 4]	; limit
	MOV		[ESP + 6], AX
	LGDT	[ESP + 6]
	RET

; void load_idt(int limit, int addr)
load_idt:
	MOV		AX, [ESP + 4]	; limit
	MOV		[ESP + 6], AX
	LIDT	[ESP + 6]
	RET

inithandle21:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	InitHandle21
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

inithandle2c:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	InitHandle2c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

; void write_mem8(int addr, int data)
write_mem8:
	MOV	ECX, [ESP + 4]	; [ESP+4]Ç…addrÇ™ì¸Ç¡ÇƒÇ¢ÇÈÇÃÇ≈ECXÇ…ì«Ç›çûÇﬁ
	MOV	AL, [ESP + 8]	; [ESP+8]Ç…dataÇ™ì¸Ç¡ÇƒÇ¢ÇÈÇÃÇ≈ALÇ…ì«Ç›çûÇﬁ
	MOV	[ECX], AL
	RET
