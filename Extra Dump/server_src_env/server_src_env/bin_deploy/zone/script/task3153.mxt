


proc 2

			
	GET_MAP_NPC_ID 302057 50 0 
	mon_speech @50 0 2 1 ������ʼ�е���Ȥ�ˣ�����һ�����������Ҳ�Ҫ���˵�̫�졣
	
endproc 
proc 4
;ʧ��
	DELETE_MON_BY_DEFID 261008 1
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50600	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 506
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
;		pworld_over
	endif
		
endproc 
proc 5
;����
	DELETE_MON_BY_DEFID 261008 1
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50600	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 506
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac		
;		pworld_over

	endif
endproc

proc 6
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 1 1 1 
	if Map != 50600
		TASK_FAIL 3153 1
		return
	endif
	;������ͼ����ʧ��


	get_task_var 3153 0 8 1 20
	if 20 = 1
		
		get_task_var 3153 1 8 1 21
		add 21 1
		set_task_var 3153 1 8 @21 1
		
		if 21 = 1
			GET_MAP_NPC_ID 302057 50 0 
			mon_speech @50 0 2 1 �һ����ٻ���������������ٻ�������
			effect 0 0 playEffct(37500,19400,"effect\\entity\\9591_16.ent")
		endif
		if 21 = 3
			call_mon 1 261008 0 0 37500 19400 2
			add_sys_msg_start
			add_sys_msg ע�⣬������һ��С�����ˣ�
			set_sys_msg_mode 3 0 0	
			msg_info 
		endif
		if 21 = 15
		
			effect 0 0 playEffct(37500,20200,"effect\\entity\\9591_16.ent")
		endif
		if 21 = 17
			call_mon 1 261008 0 0 37500 20200 2
			add_sys_msg_start
			add_sys_msg ��1������������Ԯ�ˣ�
			set_sys_msg_mode 3 0 0	
			msg_info 
		endif
		if 21 = 30
			effect 0 0 playEffct(37500,18700,"effect\\entity\\9591_16.ent")
		endif
		if 21 = 32
			call_mon 1 261008 0 0 37500 18700 2
			add_sys_msg_start
			add_sys_msg ����1������������Ԯ�ˣ�
			set_sys_msg_mode 3 0 0	
			msg_info 
		endif
		if 21 > 32
			var 20 2
			set_task_var 3153 0 8 @20 1
		endif
	endif

endproc 