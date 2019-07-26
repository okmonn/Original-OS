	ORG 0x7c00 ; プログラム読み込み位置
	JMP entry  ; entryに移動

;FAT12フォーマットのフロッピーディスク
	DB 0xeb, 0x4e, 0x90
	DB "HELLOIPL"    ; ブートセクタ名（8バイト）
	DW 512           ; 1セクタの大きさ
	DB 1             ; クラスタの大きさ
	DW 1             ; 開始セクタ番号
	DB 2             ; FAT数
	DW 224           ; ルートディレクトリ領域の大きさ
	DW 2880          ; ドライブ容量
	DB 0x0f          ; メディアタイプ
	DW 9             ; FAT領域の長さ
	DW 18            ; 1トラックに存在するセクタ数
	DW 2             ; ヘッド数
	DD 0             ; パーティション
	DD 2880          ; ドライブ容量
	DB 0, 0, 0x29    ; 
	DD 0xffffffff    ; ボリュームシリアル番号
	DB "HELLO-OS   " ; ディスク名（11バイト）
	DB "FAT12   "    ; フォーマット名（8バイト）
	TIMES 18 DB 0    ; 18バイト0埋め

entry:
	MOV AX, 0      ; アキュムレータに0を代入
	MOV SS, AX     ; スタックセグメントにアキュムレータの値を代入
	MOV SP, 0x7c00 ; スタックポインタに0x7c00を代入
	MOV DS, AX     ; データセグメントにアキュムレータの値を代入
	MOV ES, AX     ; エクストラセグメントにアキュムレータの値を代入
	MOV SI, msg    ; ソースインデックスにmsgアドレスを代入

putloop:
	MOV AL, [SI] ; アキュムレータハイにソースインデックスのメモリ代入
	ADD SI, 1    ; ソースインデックスをインクリメント
	CMP AL, 0    ; アキュムレータハイと「0」を比較
	JE fin       ; (AL == 0)ならfinに移動

	MOV AH, 0x0e ; アキュムレータハイに0x0eを代入
	MOV BX, 15   ; ベースに15を代入
	INT 0x10     ; 割り込み（ビデオ制御）
	JMP putloop  ; putloopに移動

fin:
	HLT     ; CPU待機
	JMP fin ; ループ

msg:
	DB 0x0a, 0x0a                         ; 
	DB "Hello World!"                     ; 
	DB 0x0a                               ; 
	DB 0                                  ; 

	TIMES 0x7dfe - 0x7c00 - ($ - $$) DB 0 ; 現在のアドレスから0x1feまで0埋め
	DB 0x55, 0xaa                         ; 

;ブートセクタ以外の部分
	DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 ; 
	TIMES 4600 DB 0                                   ; 
	DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 ; 
	TIMES 1469432 DB 0                                ; 
