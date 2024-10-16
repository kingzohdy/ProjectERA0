
proc 1
	var 30 0
	set_task_var 3411 0 8 @30 1
	var 31 0
	set_task_var 3411 1 8 @31 1 
endproc 


proc 6

	strvar _Rac Map
	GET_ID_PLAYER_INFO 0 7 _Rac
	get_player_mapinfo 0 Map 2 0 0
	
;;;;;;;;;;;;;;;;;;;;;
	strvar Tim1
	set_pworld_var 8 8 1 Tim1 
	if Tim1 = 0
		get_task_var 3411 0 8 1 30
		add 30 1
		set_task_var 3411 0 8 @30 1
		if 30 = 10
			strvar Mid
			GET_MAP_NPC_ID 304041 Mid 1
			mon_speech @Mid 2 2 1 选中我旁边倒在地上的身体，使用任务追踪上的匕首，多使用几次，我需要大量的血液！
			var 30 0
			set_task_var 3411 0 8 @30 1
			
		endif
	endif

endproc 

