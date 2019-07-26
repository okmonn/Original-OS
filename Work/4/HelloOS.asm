	CYLS EQU 10 ; 読み込みシリンダ数
	ORG 0x7c00  ; プログラム読み込み位置
	JMP entry   ; entryに移動

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

	;ディスクを読み込む
	MOV AX, 0x0820 ; アキュムレータに0x0820を代入
	MOV ES, AX     ; エクストラセグメントにアキュムレータの値を代入
	MOV CH, 0      ; カウンタハイに0を代入
	MOV DH, 0      ; データハイに0を代入
	MOV CL, 2      ; カウンタロウに2を代入

readloop:
	MOV SI, 0      ; 失敗回数記録

retry:
	MOV AH, 0x02 ; アキュムレータハイに0x02を代入
	MOV AL, 1    ; アキュムレータロウに1を代入
	MOV BX, 0    ; ベースに0を代入
	MOV DL, 0x00 ; データロウに0x00を代入
	INT 0x13     ; 割り込み（ディスク制御）
	JNC next     ; エラーが出なかったらnextに移動

	ADD SI, 1    ; ソースインデックスをインクリメント
	CMP SI, 5    ; ソースインデックスと「5」を比較
	JAE error    ; SI >= 5ならerrorに移動

	MOV AL, 0x00 ; アキュムレータハイに0x00を代入
	MOV DL, 0x00 ; データロウに0x00を代入
	INT 0x13     ; 割り込み（システムのリセット）
	JMP retry    ; ループ

next:
	MOV AX, ES     ; アキュムレーターにエクストラセグメントの値を代入
	ADD AX, 0x0020 ; アキュムレーターに0x0020を加算
	MOV ES, AX     ; エクストラセグメントにアキュムレータの値を代入
	ADD CL, 1      ; カウンタロウをインクリメント
	CMP CL, 18     ; カウンタロウと「18」を比較
	JBE readloop   ; CL <= 18ならreadloopに移動

	MOV CL, 1      ; カウンタロウに1を代入
	ADD DH, 1      ; データハイをインクリメント
	CMP DH, 2      ; データハイと「2」を比較
	JB readloop    ; DH < 2ならreadloopに移動

	MOV DH, 0      ; データハイに0を代入
	ADD CH, 1      ; カウンタハイをインクリメント
	CMP CH, CYLS   ; データハイとCYLSを比較
	JB readloop    ; CH < CYLSならreadloopに移動

	MOV [0x0ff0], CH ; IPLがどこまで読んだのかをメモ
	JMP 0xc200       ; OS実行

error:
	MOV AX, 0   ; アキュムレーに0を代入
	MOV ES, AX  ; エクストラセグメントにアキュムレータの値を代入
	MOV SI, msg ; ソースインデックスにmsgの値を代入

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
	DB "Load Error"                       ; 
	DB 0x0a                               ; 
	DB 0                                  ; 

	TIMES 0x7dfe - 0x7c00 - ($ - $$) DB 0 ; 現在のアドレスから0x1feまで0埋め
	DB 0x55, 0xaa                         ; 
