	CYLS EQU 10 ; いくつのシリンダを読み込むか
	ORG 0x7c00  ; プログラム読み込み開始位置
	JMP entry   ; entryに移動

	;FAT12フォーマットフロッピー
	DB 0x90
	DB "HARIBOTE"    ; ブートセクタの名前を自由に書いてよい（8バイト）
	DW 512           ; 1セクタの大きさ（512にしなければいけない）
	DB 1             ; クラスタの大きさ（1セクタにしなければいけない）
	DW 1             ; FATがどこから始まるか（普通は1セクタ目からにする）
	DB 2             ; FATの個数（2にしなければいけない）
	DW 224           ; ルートディレクトリ領域の大きさ（普通は224エントリにする）
	DW 2880          ; このドライブの大きさ（2880セクタにしなければいけない）
	DB 0xf0          ; メディアのタイプ（0xf0にしなければいけない）
	DW 9             ; FAT領域の長さ（9セクタにしなければいけない）
	DW 18            ; 1トラックにいくつのセクタがあるか（18にしなければいけない）
	DW 2             ; ヘッドの数（2にしなければいけない）
	DD 0             ; パーティションを使ってないのでここは必ず0
	DD 2880          ; このドライブ大きさをもう一度書く
	DB 0,0,0x29      ; よくわからないけどこの値にしておくといいらしい
	DD 0xffffffff    ; たぶんボリュームシリアル番号
	DB "HARIBOTEOS " ; ディスクの名前（11バイト）
	DB "FAT12   "    ; フォーマットの名前（8バイト）
	TIMES 18 DB 0    ; とりあえず18バイトあけておく

entry:
	MOV AX, 0      ; レジスタ初期化
	MOV SS, AX     ; 
	MOV SP, 0x7c00 ; 
	MOV DS, AX     ; 

	;ディスク読み込み
	MOV AX, 0x0820 ; 
	MOV ES, AX     ; 
	MOV CH, 0      ; シリンダ0
	MOV DH, 0      ; ヘッダ0
	MOV CL, 2      ; セクタ0

readloop:
	MOV SI, 0 ; 失敗回数を格納するレジスタ

retry:
	MOV AH, 0x02 ; ディスク読み込み
	MOV AL, 1    ; セクタ1
	MOV BX, 0    ; 
	MOV DL, 0x00 ; Aドライブ
	INT 0x13     ; ディスクBIOS呼び出し
	JNC next     ; エラーが起きなければnextに移動

	ADD SI, 1 ; インクリメント
	CMP SI, 5  ; SIと「5」を比較
	JAE error  ; (SI >= 5)ならerrorに移動

	MOV AH, 0x00 ; 
	MOV DL, 0x00 ; 
	INT 0x13     ; ドライブリセット
	JMP retry    ; ループ

next:
	MOV AX, ES     ; アドレスを0x200進める
	ADD AX, 0x0020 ; 
	MOV ES, AX     ; ADD ES, 0x0020の代わり
	ADD CL, 1      ; インクリメント
	CMP CL, 18     ; CLと「18」を比較
	JBE readloop   ; (CL <= 18)ならreadloopに移動

	MOV CL, 1   ; 
	ADD DH, 1   ; 
	CMP DH, 2   ; DHと「2」を比較
	JB readloop ; (DH < 2)ならreadloopに移動

	MOV DH, 0    ; 
	ADD CH, 1    ; インクリメント
	CMP CH, CYLS ; CHとCYLSを比較
	JB readloop  ; (CH < CYLS)ならreadloopに移動

	;読み終わったのでOS実行
	MOV [0x0ff0], CH ; IPLがどこまで読み込んだのかメモ
	JMP 0xc200       ; OSのアドレスへ移動

error:
	MOV AX, 0   ; 
	MOV ES, AX  ; 
	MOV SI, msg ; 

putloop:
	MOV AL, [SI] ; 
	ADD SI, 1    ; インクリメント
	CMP AL, 0    ; 
	JE fin       ; (AL > 0)ならfinに移動

	MOV AH, 0x0e ; 一文字表示ファンクション
	MOV BX, 15   ; カラーコード
	INT 0x10     ; ビデオBIOS呼び出し
	JMP putloop  ; ループ

fin:
	HLT     ; CPU待機
	JMP fin ; ループ

msg:
	DB 0x0a, 0x0a                         ; 改行
	DB "load error"                       ; 
	DB 0x0a                               ; 改行
	DB 0                                  ; 

	TIMES 0x7dfe - 0x7c00 - ($ - $$) DB 0 ; 0x7dfeまで0x00で埋める
	DB 0x55, 0xaa                         ; 
