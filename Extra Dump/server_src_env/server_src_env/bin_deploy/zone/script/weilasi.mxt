

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  �Ҹо���Խ��Խ���ˡ���


	TASK_STAT 12066 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���Ǳ���������壬����û�з������Ƶ������壿
	endif

	TASK_STAT 12266 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����������Ҫ��һЩ���ϣ���Ը���æ�ռ�һЩô��
	endif
	talk
endproc 

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  �����Ѿ����ٵ���Ƭ���أ��ֲ�������һ������һ��������

	OPTION 250 #(����)֪���ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0
	task_fini 12066 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  ��Σ�յĴ���֮�����ߣ�һֱ������ȥע����ʲô���������������������ʦҪ��Ļ��������һῼ��һ�¡�

	OPTION 260 #(����)лл���ˣ�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0
	task_fini 12266 1
	endif
ENDPROC
