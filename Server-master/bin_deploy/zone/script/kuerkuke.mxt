

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bԽ�Ǿ޴��ð�գ�Խ����Ҫ�޴�Ľ��������뵨С��ų���޹ء�
	task_add_option 1503 1 101
	task_add_option 1536 1 102

	task_add_option 1537 1 103
	task_add_option 1538 1 104 
	
	task_add_option 1507 1 106


	TASK_STAT 12035 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)����û�п��������Ƶ�ϸ�������尡��
	endif
	TASK_STAT 12235 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)���Ǹ�����ţ��򿪿�����
	endif
	talk
endproc 


proc 101
    task_do_option 1503 1
endproc
proc 102
    task_do_option 1536 1
endproc
proc 103
    task_do_option 1537 1
endproc
proc 104
    task_do_option 1538 1
endproc
proc 106
    task_do_option 1507 1
endproc


PROC 150	
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��������Ϣ�Ѿ�����������ɳĮ����������ֻ�������������������ɼ�԰��

	OPTION 250 #(����)�һ�������ǵģ�
	TALK
ENDPROC
proc 250
	
	GET_ITEM_NUM_INPAK 5300504 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300504 1 0
    	task_fini 12035 1
	endif
endproc

proc 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b���ʵ���Ѫ�Ѿ���Ϊ��Щ���������׷�������Ѿ�����ֻ�֮���ˡ�

	OPTION 260 #(����)����һ����취��
	TALK
endproc
proc 260
	
	GET_ITEM_NUM_INPAK 5300514 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300514 1 0
    	task_fini 12235 1
	endif
endproc
;proc 2
;	get_trig_task 1 2
;	if 2 = 1210
;		task_fini 1210 1
;	endif
;
;endproc 