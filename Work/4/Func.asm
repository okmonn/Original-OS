section .text
	global io_hlt, io_cli, io_sti, io_stihlt
	global io_in8, io_in16, io_in32
	global io_out8, io_out16, io_out32
	global io_load_eflags, io_store_eflags

io_hlt:
	HLT ; CPU待機
	RET ; 関数終了

io_cli:
	CLI ; IFフラグをクリアし割り込み禁止
	RET ; 関数終了

io_sti: ; 
	STI ; 割り込みフラグのセット
	RET ; 関数終了

io_stihlt:
	STI ; 割り込みフラグのセット
	HLT ; CPU待機
	RET ; 関数終了

io_in8:
	MOV EDX, [ESP + 4] ; 
	MOV EAX, 0         ; 
	IN AL, DX          ; 
	RET                ; 

io_in16:
	MOV EDX, [ESP + 4] ; 
	MOV EAX, 0         ; 
	IN AX, DX          ; 
	RET                ; 

in_in32:
	MOV EDX, [ESP + 4]  ; 
	IN EAX, DX          ; 
	RET                 ; 

io_out8:
	MOV EDX, [ESP + 4] ; 
	MOV AL, [ESP + 8]  ; 
	OUT DX, AL         ; 
	RET                ; 

io_out16:
	MOV EDX, [ESP + 4] ; 
	MOV EAX, [ESP + 8] ; 
	OUT DX, AX         ; 
	RET                ; 

io_out32:
	MOV EDX, [ESP + 4] ; 
	MOV EAX, [ESP + 8] ; 
	OUT DX, EAX        ; 
	RET                ; 

io_load_eflags:
	PUSHFD
	POP EAX
	RET

io_store_eflags:
	MOV EAX, [ESP + 4]
	PUSH EAX
	POPFD
	RET
