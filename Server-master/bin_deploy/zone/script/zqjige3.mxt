proc 1


	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK  #b#b����Ϊ�������̻�ĳ�Ա���Ҿ�Ȼ���䵽�������������ծ�����裡̫�����ˣ�������ҳ���Ҫ����Ķ�Ӯ������

	task_add_option 3133 1 100
	task_add_option 3134 1 101

	task_add_option 3135 1 102
	
	get_task_var 3135 5 8 1 12
		
	TASK_STAT 3135 1 20
	if 20 = 2 and 12 = 0
		option 150 ��ʼ����
	
	endif
;	option 200 #������Ч
	
	TALK
	
endproc
proc 200
	effect 0 0 playEffct(31600,45000,"effect\\entity\\9548_16.ent")
endproc

PROC 100
	task_do_option 3133 1
ENDPROC
PROC 101
	task_do_option 3134 1
ENDPROC
PROC 102
	task_do_option 3135 1
ENDPROC
proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ҿ�����ǿ�����������ٶ���ô����ϣ����û�а�æ�
		option 151 #��ʼ�ɣ�
		option 152 #����
	talk
endproc
proc 151
	
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	strvar mid
	GET_ROLE_RIDE_ON_MACHINE 0 mid
	if mid != 0
		add_sys_msg_start
		add_sys_msg ���ɼ�ʻ��е���д�����
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if 35 = 0 
	
	
		get_task_var 3135 0 8 1 11
		var 11 1
		set_task_var 3135 0 8 @11 1
		
		get_task_var 3135 5 8 1 12
		var 12 1
		set_task_var 3135 5 8 @12 1
		
		GET_PLAYER_ID 1 31
		
		TRANSMIT_CAMERA @31 45 1000 31500 44700 0 0
		;����Ҵ��͵�һ��λ��	
		TARGET_ADD_STATUS 1 @31 3099 1 5 1	
		
		;ˢ����mon
	;	NEW_MON_BY_DIR 204033 1 31600 44900 0 57 0 45 0 0
		strvar monmid
		CALL_MON_BY_PATROL 1 204033 0 0 31600 44900 57 45 monmid
		
		
		get_task_var 3135 7 32 1 10
		var 10 @monmid
		set_task_var 3135 7 32 @10 1
		
		effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9568_16.ent")	
		effect 2 0 playEffct(31300,44700,"effect\\entity\\9569_16.ent")
		
	else 
		add_sys_msg_start
		add_sys_msg �����û������������
		set_sys_msg_mode 4 0 0
		msg_info
;		TASK_FAIL 3135 1
		return 
	endif



endproc
proc 152
	return
endproc
proc 2

	get_trig_task 1 2
	if 2 = 3135
		DIALOG 1
		ADD_TALK $_npc$:       
		ADD_TALK  #b#b�������ǿ׳�����ܲ��ɣ�������뽡�������ܲ��ɣ������������������ھͿ�ʼ�ܲ��ɣ�
			option 150 ��ʼ����
		talk
	endif

endproc 

