
proc 1



endproc

proc 4
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 1 1 1 
	if Map = 51300
;		strvar x y
;		GET_ID_PLAYER_INFO 0 3 x
;		GET_ID_PLAYER_INFO 0 4 y	
;		new_npc 1 333020 @x @y 0 jlgozc.mac
;		
		TRANSMIT 1000 20900 17000 1
;		add_sys_msg_start
;		add_sys_msg ����֮��������Ѿ��������������Χ���������������ɣ�
;		set_sys_msg_mode 3 0 0
;		msg_info
;		
	endif
endproc 

proc 5
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 1 1 1 
	if Map = 51300
;		strvar x y
;		GET_ID_PLAYER_INFO 0 3 x
;		GET_ID_PLAYER_INFO 0 4 y	
;		new_npc 1 333020 @x @y 0 jlgozc.mac

		TRANSMIT 1000 20900 17000 1
	endif
endproc 

proc 6


	strvar locmon map

	GET_PLAYER_MAPINFO 0 map 0 0 0

	if map != 51300
		return
	endif
	
	strvar x y
	GET_ID_PLAYER_INFO 0 3 x
	GET_ID_PLAYER_INFO 0 4 y	
	
	get_pworld_var 1 8 1 locmon

	if locmon = 10
		;����ﵽ8ֻˢ��е

		new_mon_machine_by_dir 1 290048 0 1 @x @y 0 0
		add_sys_msg_start
		add_sys_msg �����������Ͻ����ϣ��������ٶ�ɨƽ���ˣ�
		set_sys_msg_mode 3 0 0
		msg_info
		
		var locmon 0
		set_pworld_var 1 8 @locmon 1
		
	endif
	strvar Retid
	GET_ROLE_RIDE_ON_MACHINE 0 Retid 
	
	strvar locq
	get_pworld_var 2 8 1 locq
	if Retid = 0
		;δ��˱�־
		var locq 0
		set_pworld_var 2 8 @locq 1
	else
		;��˱�־
		if Retid = 290048
			var locq 10
			set_pworld_var 2 8 @locq 1
		else
			var locq 0
			set_pworld_var 2 8 @locq 1
		endif
	endif
	
	;����ﵽ20ֻʧ��
	strvar locmonum
	get_pworld_var 3 8 1 locmonum 
	if locmonum = 1
		TASK_FAIL 1092 1
	endif
	if locmonum = 2
		task_fini 1092 1
		new_npc 1 333020 @x @y 0 jlgozc.mac
	endif
endproc  






