proc 1


	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK  #b#b�ߣ��ɶ�ĵ�����϶��ڴ��ʱ��ħ���ˣ�������ҷ��־������������Ҿ�Ȼ����������������Ĳ��У�̫�������ˣ�

	task_add_option 3132 1 100
	task_add_option 3133 1 101
	
	get_task_var 3133 5 8 1 12
	TASK_STAT 3133 1 20
	if 20 = 2 and 12 = 0
		option 150 ��ʼ����
		
	endif
	TALK
	
endproc
PROC 100
	task_do_option 3132 1
ENDPROC

PROC 101
	task_do_option 3133 1

ENDPROC
proc 150

	dialog 1
	add_talk $_npc$:
	add_talk #b#b׼�������𣡣�
		option 151 #��ʼ�ɣ�
		option 152 #�ҹ������ܲ������������˰ɣ�
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
		get_task_var 3133 0 8 1 11
		var 11 1
		set_task_var 3133 0 8 @11 1
		
		get_task_var 3133 5 8 1 12
		var 12 1
		set_task_var 3133 5 8 @12 1
		GET_PLAYER_ID 1 31
		TRANSMIT_CAMERA @31 90 1000 31700 34600 0 0
		;����Ҵ��͵�һ��λ��	
		TARGET_ADD_STATUS 1 @31 3099 1 4 1	
		;ˢ����mon
		strvar monmid
		NEW_MON_BY_DIR 204032 1 32000 34600 0 56 0 90 0 monmid
		
		
		
		get_task_var 3133 5 32 0 10 
		var 10 @monmid
		set_task_var 3133 5 32 @10 0
		
		effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9568_16.ent")		
		effect 0 0 playEffct(31700,35000,"effect\\entity\\9570_16.ent")
	else 
		add_sys_msg_start
		add_sys_msg ��Ӧ�����������
		set_sys_msg_mode 4 0 0
		msg_info
;		TASK_FAIL 3133 1
		return 
	endif

endproc 
proc 152
	 return
endproc 
proc 2

	get_trig_task 1 2

	if 2 = 3133	
;		GET_PLAYER_ID 30 0
;;		PLAYER_RIDE_ON @30 40004 35
;		GET_ITEM_NUM_INPAK 5600004 1 40
;		if 40 < 1 and 35 != 0
;			ADD_ITEM_NUM_INPAK 5600004 1 1 0
;		endif
		dialog 1
		add_talk $_npc$:
		add_talk #b#b׼�������𣡣�
			option 151 #��ʼ�ɣ�
			option 152 #�ҹ������ܲ������������˰ɣ�
			
		talk
	
	endif 

endproc 


