proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ų�����̬�ȴ��˽������̹�ϵ������������ˣ�����Ϊ�������룬�Ǳ��˶����Ǿ�û���κ����������һ������Ȼ�����������ˡ�

	task_add_option 1306 1 101
	task_add_option 1307 1 102
	task_add_option 1308 1 103
	task_add_option 1329 1 104

	task_stat 3103 0 20
	if 20 = 2
		option 200 �����׾�ʯ
		
	endif
	
	
	TASK_STAT 3225 1 20
	TASK_STAT 3255 1 21
	TASK_STAT 3285 1 22
	TASK_STAT 3517 1 23
	TASK_STAT 3539 1 24
	TASK_STAT 3563 1 25
	TASK_STAT 3609 1 26
	if 21 = 2 or 20 = 2 or 22 = 2 or 23 = 2 or 24 = 2 or 25 = 2 or 26 = 2
	  	option 150 ��������-����Э������
	endif
	
	talk
endproc
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk   <���������Щ��ŭ�Ŀ����㣬�����룺˭������Щ�һ����������׾�ʯ�ģ�����������ÿ����������������ˣ����ˣ�����������һ����Ҫ�ⶫ���İɣ�����һ��Ҳ�޷����Ͼ���˵�Ѿ�Խ��Խ���ˡ�>
	add_talk ���������׾�ʯ����������Щͬ�飬�������һ���ˣ�������Ҳû���׾�ʯ�ˡ�

	
	option 201 #лл��
	talk
endproc
proc 201
	task_fini 3103 0
	return
endproc



PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#bлл���ǵİ�������������һ������ת������ǵ������ɰͣ�
		
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
		TASK_STAT 3225 1 70
		TASK_STAT 3255 1 71
		TASK_STAT 3285 1 72
		TASK_STAT 3517 1 73
		TASK_STAT 3539 1 74
		TASK_STAT 3563 1 75
		TASK_STAT 3609 1 76
		if 70 = 2
			task_fini 3225 1
		endif
		if 71 = 2
			task_fini 3255 1
		endif
		if 72 = 2
			task_fini 3285 1
		endif
		if 73 = 2
			task_fini 3517 1
		endif
		if 74 = 2
			task_fini 3539 1
		endif
		if 75 = 2
			task_fini 3563 1
		endif
		if 76 = 2
			task_fini 3609 1
		endif
	endif
ENDPROC




proc 101
    task_do_option 1306 1
endproc
proc 102
    task_do_option 1307 1
endproc
proc 103
    task_do_option 1308 1
endproc
proc 104
    task_do_option 1329 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1308
		task_fini 1308 1
	endif

endproc

