proc 1
	dialog 1
	ADD_TALK ����ָ��ֿ⡢����
	option 12 #�ֿ�
	option 13 #�������
	option 14 #��̬����
	option 15 ����
	option 16 װ���ӹ�
	option 17 ��ʯ����
	option 11 #�ټ�	
	TALK
endproc

proc 11
	return
endproc

proc 12
	open_ui 0
endproc

proc 13
	open_ui 4
endproc

proc 14
	open_pworld_ui 503
endproc

proc 15
	dialog 1
	ADD_TALK �������
	option 51 ��������
	option 52 ����/����/�����������
	option 39 #�������
	option 40 ��������
	option 1 #����
	TALK
endproc

proc 22
	open_ui 6
endproc

proc 23
	open_ui 7
endproc

proc 24
	open_ui 8
endproc

proc 25
	open_ui 9
endproc

proc 16
	dialog 1
	ADD_TALK װ���ӹ�
	option 32 #װ��Ʒ������
	option 33 #װ��ս��������
	option 34 #װ���ȼ�����
	option 35 #װ������
	option 1 ����
	TALK
endproc

proc 17
	dialog 1
	ADD_TALK ��ʯ����
	option 36 #��ʯ����
	option 37 #��ʯ��Ƕ
	option 38 #��ʯժȡ
	option 1 ����
	TALK
endproc

proc 32
	open_ui 10
endproc

proc 33
	open_ui 11
endproc

proc 34
	open_ui 12
endproc

proc 35
	open_ui 13
endproc

proc 36
	open_ui 14
endproc

proc 37
	open_ui 15
endproc

proc 38
	open_ui 16
endproc

proc 39
	enter_clan_city
endproc

proc 40
	dialog 1
	ADD_TALK ������������
	option 41 ����������
	option 42 ����������
	; option 45 #���Ű���Ԫ
	; option 46 #���ž���ֵ
	option 47 ���ڹ���������
	option 48 ���ڳ���������
	TALK
endproc

proc 41
	dialog 1
	ADD_TALK ��ǰ����������
	dynamic_option_list 0 1 43
	TALK
endproc

proc 42
	dialog 1
	ADD_TALK ��ǰ�������������
	dynamic_option_list 1 1 44
	TALK
endproc

proc 43
	dynamic_do_task_option 41
endproc

proc 44
	dynamic_do_task_option 42
endproc

; proc 45
	; clan_pay 0 1
; endproc

; proc 46
	; clan_pay 1 1
; endproc

proc 47
	dialog 1
	ADD_TALK ��ǰ��ѡ����������
	dynamic_option_list 2 1 49
	TALK
endproc

proc 48
	dialog 1
	ADD_TALK ��ǰ��ѡ�������������
	dynamic_option_list 3 1 50
	TALK
endproc

proc 49
	dynamic_do_task_option 40
endproc

proc 50
	dynamic_do_task_option 40
endproc

proc 51
	dialog 1
	ADD_TALK �����������
	option 22 #��������
	option 23 #��ɢ����
	option 24 #��������
	option 25 #�޸�����
	option 15 ����
	TALK
endproc

proc 52
	dialog 1
	ADD_TALK ����/����/�����������
	option 53 #�����������
	option 54 #��������������
	option 55 #�������������
	option 56 #������������
	option 57 #�������������
	option 58 #�����Ƽ������
	option 1 #����
	TALK
endproc

proc 53
	open_ui 20
endproc

proc 54
	open_ui 18
endproc

proc 55
	open_ui 21
endproc

proc 56
	open_ui 22
endproc

proc 57
	open_ui 23
endproc

proc 58
	open_ui 24
endproc
