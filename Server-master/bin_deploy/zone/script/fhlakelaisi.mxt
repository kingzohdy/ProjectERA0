; ������˹


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ѵı��ѣ��������ɰ������Ƭ��δ���Ĵ̡�Ȫˮ����ɫ��ζ�Ķ�����
		task_add_option 1941 1 100
		task_add_option 1942 1 101
		strvar Ret
		task_stat 1904 1 Ret
		if Ret = 2
			option 130 #���������
		endif	
		
		TASK_STAT 3614 1 21
		if 21 = 2
			option 150 ��������-����Э������
		endif
		

	talk
endproc

PROC 100
	task_do_option 1941 1
ENDPROC
PROC 101
	task_do_option 1942 1
ENDPROC

proc 130
;����·��
;	NEW_OWNER_TMP_ITEM 1460026 35535 26728 0 0 0 135 0 0 0 1
	strvar Ret
	AUTOFLY 116 Ret
	if Ret = 0
		TASK_FINI 1904 1
	endif
	
endproc
PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#b���Ҹ�л������������������Щѩ����̼�����ʣ��������ǻ��ܼ�������š�
	OPTION 250 #����Э������
	TALK
ENDPROC

PROC 250
	
	GET_ITEM_NUM_INPAK 5300533 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300533 1 0
		
		TASK_STAT 3614 1 71
		
		if 71 = 2
			task_fini 3614 1
		endif
	
	endif
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1941
		autofly 117 1	
		NEW_OWNER_TMP_ITEM 1460028 0 0 0 0 0 0 1095 1 3600 1

		
	endif
	if 2 = 1942
		NEW_OWNER_TMP_ITEM 1460027 35535 26728 0 0 0 136 1095 1 3600 1
	endif
endproc
