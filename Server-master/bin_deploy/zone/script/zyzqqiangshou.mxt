;ǹ������ʦ������

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������侲����ϡ�
		
		task_add_option 3161 1 101
		task_add_option 3153 1 102
		task_add_option 3162 1 103
		
		task_stat 3153 1 30
		get_task_var 3153 0 8 1 20
		if 30 = 2 and 20 = 0
			option 160 #����һ������С������
		endif
		
		task_stat 3162 1 30
		get_task_var 3162 0 8 1 20
		if 30 = 2 and 20 = 0
			option 161 #��������������ʬȮ
		endif
		
		option 152 #�ص�ʱ���
	talk
endproc

PROC 101
	task_do_option 3161 1
ENDPROC
PROC 102
	task_do_option 3153 1
ENDPROC
PROC 103
	task_do_option 3162 1
ENDPROC


proc 152
	transmit 1000 23800 27400 1
;	RESET_PWORLD 506
	task_stat 3153 1 30
	task_stat 3162 1 31
	if 30 = 2 
		TASK_FAIL 3153 1
	endif
	if 31 = 2
		TASK_FAIL 3162 1	
	endif

endproc
proc 160
	get_task_var 3153 0 8 1 20
	var 20 1
	set_task_var 3153 0 8 @20 1
	
	GET_MAP_NPC_ID 302057 50 0 
	mon_speech @50 0 2 1 �ɹ���ɱ����������ǰ������6�ξ���ʧ�ܣ���ֻ����������̫�졣
	add_sys_msg_start
	add_sys_msg ���Ժ���ʹ�á�������������������ѷ�����ǣ�ƻ�ɱ���������ˡ�
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 161
		
;		NEW_MON 262001 5 37500 19400 500 0 0 0 0
;		NEW_MON 262001 5 37500 20200 500 0 0 0 0
;		NEW_MON 262001 5 37500 18700 500 0 0 0 0
		get_task_var 3162 0 8 1 20
		var 20 1
		set_task_var 3162 0 8 @20 1
		
		GET_MAP_NPC_ID 302057 50 90
		mon_speech @50 91 2 1 ʮ���ڻ�ɱʮֻ����ʬȮ������������ӣ�������̫�����ˡ�
		
		add_sys_msg_start
		add_sys_msg ����������ʬȮ������һ����á��������䡱�����ڡ���10���ڽ���ս����
		set_sys_msg_mode 1 0 0
		msg_info
endproc

proc 2
	get_trig_task 1 2
	if 2 = 3153
		
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��������侲����ϡ�
		
			option 160 #����һ������С������
			
		talk
	endif 
	if 2 = 3162

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��������侲����ϡ�
		
			option 161 #��������������ʬȮ
			
		talk
			
	endif

endproc








