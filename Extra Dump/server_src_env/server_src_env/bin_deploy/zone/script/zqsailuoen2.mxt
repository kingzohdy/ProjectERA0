proc 1

	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b¬����Ψһ���������ܹ�������ǰ����ˣ�
	get_task_var 3132 5 8 1 12
	
		TASK_STAT 3132 1 20
		if 20 = 2 and 12 = 0
		
			option 100 ��Ҫ��ս��
		endif
	talk

endproc 
proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #b#b׼������ô��
		option 101 #������ʼ�ɣ�
		option 102 #����
	talk
endproc 

proc 101

	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	if 35 != 0 
		get_task_var 3132 0 8 1 11
		var 11 1
		set_task_var 3132 0 8 @11 1
					
		get_task_var 3132 5 8 1 12
		var 12 1
		set_task_var 3132 5 8 @12 1
		
		GET_PLAYER_ID 1 30
		TRANSMIT_CAMERA @30 135 1000 20817 34000 0 0
		;����Ҵ��͵�һ��λ��	
		TARGET_ADD_STATUS 1 @30 1006 11 4 1	

		;ˢ����monq
	;	NEW_MON 204031 1 20817 34385 0 55 0 0 0
		NEW_MON_BY_DIR 204031 1 20817 34385 0 55 0 135 0 0
		;������Ч
				
	;	NPC_DIE 0 1
		
		effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9568_16.ent")	

		effect 2 0 playEffct(21300,34100,"effect\\entity\\9571_16.ent")
	else
			
		add_sys_msg_start
		add_sys_msg ����Ҫ�ȴ���������������
		set_sys_msg_mode 4 0 0
		msg_info	
		
	endif
endproc 
proc 102
	return
endproc 

