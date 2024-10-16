
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
;		add_sys_msg 逆往之灵的数量已经超出你的能力范围，还是重新来过吧！
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
		;怪物达到8只刷机械

		new_mon_machine_by_dir 1 290048 0 1 @x @y 0 0
		add_sys_msg_start
		add_sys_msg 驱龙来助！赶紧骑上，用最快的速度扫平敌人！
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
		;未骑乘标志
		var locq 0
		set_pworld_var 2 8 @locq 1
	else
		;骑乘标志
		if Retid = 290048
			var locq 10
			set_pworld_var 2 8 @locq 1
		else
			var locq 0
			set_pworld_var 2 8 @locq 1
		endif
	endif
	
	;怪物达到20只失败
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






