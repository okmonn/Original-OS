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

;�v���O�����{��
	DB 0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c ; 
	DB 0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a ; 
	DB 0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09 ; 
	DB 0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb ; 
	DB 0xee, 0xf4, 0xeb, 0xfd                         ; 

;���b�Z�[�W����
	DB 0x0a, 0x0a               ; ���s
	DB "Hello World!"           ; ���b�Z�[�W
	DB 0x0a                     ; ���s
	DB 0

	TIMES 0x1fe - ($ - $$) DB 0 ; ���݂̃A�h���X����0x1fe�܂�0����
	DB 0x55, 0xaa               ; 

;�u�[�g�Z�N�^�ȊO�̕���
	DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 ; 
	TIMES 4600 DB 0                                   ; 
	DB 0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 ; 
	TIMES 1469432 DB 0                                ; 
