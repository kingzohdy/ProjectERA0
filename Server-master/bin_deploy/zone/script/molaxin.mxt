

proc 1
	STRVAR id a m1
	GET_TASK_VAR 1731 0 8 1 m1
	DIALOG 1
	
	GET_PLAYER_ID 1 id
	HAS_STATUS  1219 1
	IF 1 = 0
		TARGET_ADD_STATUS 1 @id 1219 1 1 1
	ENDIF
	
	
	add_talk $_npc$:
	add_talk  #b#b���������е���������΢���ƽ�������棬��Ϊ��������ˣ��㲻��Ҫ�˽�����ƽ�⣬��Ҫ������������������е����Ϻ���Ծ�Ļ�����

	task_add_option 1732 1 11
	task_add_option 1733 1 12
	task_add_option 1734 1 13
	task_add_option 1735 1 14
	task_add_option 1736 1 15
	task_add_option 1739 1 16
	task_add_option 1740 1 17
	
	TASK_STAT 12064 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���Ǳ���������壬����û�з����������ֵ������壿
	endif


	TASK_STAT 12264 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����������Ҫ��һЩ���ϣ���Ը���æ�ռ�һЩô��
	endif
	
	TASK_STAT 1731 1 a
		if a = 2 and m1 = 0
		OPTION 301 #�������������
		endif
	talk
endproc 

PROC 11
	task_do_option 1732 1
ENDPROC
PROC 12
	task_do_option 1733 1
ENDPROC
PROC 13
	task_do_option 1734 1
ENDPROC
PROC 14
	task_do_option 1735 1
ENDPROC
PROC 15
	task_do_option 1736 1
ENDPROC
PROC 16
	task_do_option 1739 1
ENDPROC
PROC 17
	task_do_option 1740 1
ENDPROC


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
	task_fini 12064 1
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
	task_fini 12264 1
	endif
ENDPROC
PROC 301
	GET_TASK_VAR 1731 0 8 1 m1
	if m1 = 0
	
		DEC_ITEM_NUM_INPAK 5300476 1 1
		if 1 = -1
			add_sys_msg_start
			add_sys_msg ��û���⣬���ҳ�ʲô��
			set_sys_msg_mode 4 0 0
			msg_info		
		else
			var @m1 1
			set_task_var 1731 0 8 @m1 1
				if 1 = -1
					return
				endif
		endif
	endif	
ENDPROC
