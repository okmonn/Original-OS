	CYLS EQU 10 ; �ǂݍ��݃V�����_��
	ORG 0x7c00  ; �v���O�����ǂݍ��݈ʒu
	JMP entry   ; entry�Ɉړ�

;FAT12�t�H�[�}�b�g�̃t���b�s�[�f�B�X�N
	DB 0xeb, 0x4e, 0x90
	DB "HELLOIPL"    ; �u�[�g�Z�N�^���i8�o�C�g�j
	DW 512           ; 1�Z�N�^�̑傫��
	DB 1             ; �N���X�^�̑傫��
	DW 1             ; �J�n�Z�N�^�ԍ�
	DB 2             ; FAT��
	DW 224           ; ���[�g�f�B���N�g���̈�̑傫��
	DW 2880          ; �h���C�u�e��
	DB 0x0f          ; ���f�B�A�^�C�v
	DW 9             ; FAT�̈�̒���
	DW 18            ; 1�g���b�N�ɑ��݂���Z�N�^��
	DW 2             ; �w�b�h��
	DD 0             ; �p�[�e�B�V����
	DD 2880          ; �h���C�u�e��
	DB 0, 0, 0x29    ; 
	DD 0xffffffff    ; �{�����[���V���A���ԍ�
	DB "HELLO-OS   " ; �f�B�X�N���i11�o�C�g�j
	DB "FAT12   "    ; �t�H�[�}�b�g���i8�o�C�g�j
	TIMES 18 DB 0    ; 18�o�C�g0����

entry:
	MOV AX, 0      ; �A�L�������[�^��0����
	MOV SS, AX     ; �X�^�b�N�Z�O�����g�ɃA�L�������[�^�̒l����
	MOV SP, 0x7c00 ; �X�^�b�N�|�C���^��0x7c00����
	MOV DS, AX     ; �f�[�^�Z�O�����g�ɃA�L�������[�^�̒l����

	;�f�B�X�N��ǂݍ���
	MOV AX, 0x0820 ; �A�L�������[�^��0x0820����
	MOV ES, AX     ; �G�N�X�g���Z�O�����g�ɃA�L�������[�^�̒l����
	MOV CH, 0      ; �J�E���^�n�C��0����
	MOV DH, 0      ; �f�[�^�n�C��0����
	MOV CL, 2      ; �J�E���^���E��2����

readloop:
	MOV SI, 0      ; ���s�񐔋L�^

retry:
	MOV AH, 0x02 ; �A�L�������[�^�n�C��0x02����
	MOV AL, 1    ; �A�L�������[�^���E��1����
	MOV BX, 0    ; �x�[�X��0����
	MOV DL, 0x00 ; �f�[�^���E��0x00����
	INT 0x13     ; ���荞�݁i�f�B�X�N����j
	JNC next     ; �G���[���o�Ȃ�������next�Ɉړ�

	ADD SI, 1    ; �\�[�X�C���f�b�N�X���C���N�������g
	CMP SI, 5    ; �\�[�X�C���f�b�N�X�Ɓu5�v���r
	JAE error    ; SI >= 5�Ȃ�error�Ɉړ�

	MOV AL, 0x00 ; �A�L�������[�^�n�C��0x00����
	MOV DL, 0x00 ; �f�[�^���E��0x00����
	INT 0x13     ; ���荞�݁i�V�X�e���̃��Z�b�g�j
	JMP retry    ; ���[�v

next:
	MOV AX, ES     ; �A�L�������[�^�[�ɃG�N�X�g���Z�O�����g�̒l����
	ADD AX, 0x0020 ; �A�L�������[�^�[��0x0020�����Z
	MOV ES, AX     ; �G�N�X�g���Z�O�����g�ɃA�L�������[�^�̒l����
	ADD CL, 1      ; �J�E���^���E���C���N�������g
	CMP CL, 18     ; �J�E���^���E�Ɓu18�v���r
	JBE readloop   ; CL <= 18�Ȃ�readloop�Ɉړ�

	MOV CL, 1      ; �J�E���^���E��1����
	ADD DH, 1      ; �f�[�^�n�C���C���N�������g
	CMP DH, 2      ; �f�[�^�n�C�Ɓu2�v���r
	JB readloop    ; DH < 2�Ȃ�readloop�Ɉړ�

	MOV DH, 0      ; �f�[�^�n�C��0����
	ADD CH, 1      ; �J�E���^�n�C���C���N�������g
	CMP CH, CYLS   ; �f�[�^�n�C��CYLS���r
	JB readloop    ; CH < CYLS�Ȃ�readloop�Ɉړ�

	MOV [0x0ff0], CH ; IPL���ǂ��܂œǂ񂾂̂�������
	JMP 0xc200       ; OS���s

error:
	MOV AX, 0   ; �A�L�������[��0����
	MOV ES, AX  ; �G�N�X�g���Z�O�����g�ɃA�L�������[�^�̒l����
	MOV SI, msg ; �\�[�X�C���f�b�N�X��msg�̒l����

putloop:
	MOV AL, [SI] ; �A�L�������[�^�n�C�Ƀ\�[�X�C���f�b�N�X�̃��������
	ADD SI, 1    ; �\�[�X�C���f�b�N�X���C���N�������g
	CMP AL, 0    ; �A�L�������[�^�n�C�Ɓu0�v���r
	JE fin       ; (AL == 0)�Ȃ�fin�Ɉړ�
	
	MOV AH, 0x0e ; �A�L�������[�^�n�C��0x0e����
	MOV BX, 15   ; �x�[�X��15����
	INT 0x10     ; ���荞�݁i�r�f�I����j
	JMP putloop  ; putloop�Ɉړ�

fin:
	HLT     ; CPU�ҋ@
	JMP fin ; ���[�v

msg:
	DB 0x0a, 0x0a                         ; 
	DB "Load Error"                       ; 
	DB 0x0a                               ; 
	DB 0                                  ; 

	TIMES 0x7dfe - 0x7c00 - ($ - $$) DB 0 ; ���݂̃A�h���X����0x1fe�܂�0����
	DB 0x55, 0xaa                         ; 
