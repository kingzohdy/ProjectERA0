proc 1
	STRVAR time a id playerid
	var time 0
	set_task_var 1746 0 8 @time 1
	var a 0
	set_task_var 1746 1 8 @a 1
	
	
	
	;effect 2 0 startGray()
	;CALL_MON 1 235012 0 120 20400 33200 id
	;set_task_var 1746 2 32 @id 1
		;if 1 = -1
		;	return
		;else
		;	add_sys_msg_start
		;	add_sys_msg 蛇神出现了，快去保护蛇神，绝对不能让仪式被打断！
		;	set_sys_msg_mode 3 0 0
		;	msg_info
		;endif
ENDPROC

proc 2
	effect 2 0 stopGray()
	;GET_PLAYER_ID 1 playerid
	;TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
	get_task_var 1746 2 32 1 id
	SET_MON_CURR @id 1 
	DESTROY_MON 1
	
	DELETE_MON_BY_DEFID 205017 1
	DELETE_MON_BY_DEFID 205018 1
ENDPROC
proc 3
	effect 2 0 stopGray()

endproc 
proc 4
	effect 2 0 stopGray()
	;GET_PLAYER_ID 1 playerid
	;TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
	get_task_var 1746 2 32 1 id
	DEL_ID_MON @id 1
;	SET_MON_CURR @id 1 
;	DESTROY_MON 1
		
	DELETE_MON_BY_DEFID 205017 1
	DELETE_MON_BY_DEFID 205018 1
ENDPROC
proc 5
	effect 2 0 stopGray()
	;GET_PLAYER_ID 1 playerid
	;TARGET_ADD_STATUS 1 @playerid 1018 1 1 1
	get_task_var 1746 2 32 1 id
	SET_MON_CURR @id 1 
	DESTROY_MON 1
	
	DELETE_MON_BY_DEFID 205017 1
	DELETE_MON_BY_DEFID 205018 1		
	
ENDPROC

proc 6
	get_task_var 1746 0 8 1 time
	get_task_var 1746 1 8 1 a
	get_task_var 1746 2 32 1 id
		if a = 1
			TASK_FAIL 1746 1
		endif
		if a = 2
			add time 1
			set_task_var 1746 0 8 @time 1	
			SET_MON_CURR @id 1
				if time = 15 
					MON_CALL_ENEMY_MON 205017 1 55 1 20400 33200 500 0 0 0 300 1 0
					MON_CALL_ENEMY_MON 205018 1 55 1 20400 33200 500 0 0 0 300 1 0
				endif
				if time = 30 
					MON_CALL_ENEMY_MON 205017 1 55 1 20400 33200 500 0 0 0 300 1 0
					MON_CALL_ENEMY_MON 205018 1 55 1 20400 33200 500 0 0 0 300 1 0
				endif
				if time = 45 
					MON_CALL_ENEMY_MON 205017 2 55 1 20400 33200 500 0 0 0 300 1 0
					MON_CALL_ENEMY_MON 205018 2 55 1 20400 33200 500 0 0 0 300 1 0
				endif
				
				if time = 60 

					if 1 = -1
						return
					else
						SET_MON_CURR @id 1 
						DESTROY_MON 1
						task_fini 1746 1	
						effect 2 0 stopGray()
					endif
				endif
				strvar Map
				GET_PLAYER_MAPINFO 2 Map 2 2 2 
				if Map != 2500
					TASK_FAIL 1746 2
				endif
		endif
endproc
