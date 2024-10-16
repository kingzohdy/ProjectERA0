proc 1
	strvar Loc1
	var Loc1 0
	set_task_var 3161 8 8 @Loc1 1
	;进入副本后开始检查地图

endproc 

proc 6

	get_task_var 3161 8 8 1 Loc1
	if Loc1 = 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 1 1 1 
		if Map != 50600
			TASK_FAIL 3161 1
			return
		endif
	endif
	;传出地图任务失败
	
endproc
