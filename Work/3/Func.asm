section .text
	global _Wait

_Wait:
	HLT ; CPU待機
	RET ; 関数終了
