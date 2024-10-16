;»¤ËÍÊ¿±ø

proc 1
	strvar Mid1 lock 
	var Mid1 0
	set_task_var 1966 0 32 @Mid1 1
	
	var lock 0
	set_task_var 1966 4 32 @lock 1

endproc 
proc 5
	
	get_task_var 1966 0 32 1 Mid1
	DEL_ID_MON @Mid1 1
endproc 

proc 6
	strvar M pX pY Mx My Map
	GET_PLAYER_MAPINFO 0 Map 0 pX pY
	get_task_var 1966 0 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx My	
	
	
	GET_POINT_DIST 6700 21500 @Mx @My M
	if M < 300
		task_fini 1966 1
		get_task_var 1966 0 32 1 Mid1
		DEL_ID_MON @Mid1 1
		
	endif
	
	
	get_task_var 1966 4 32 1 lock
	if Map != 3200 and lock != 0
		
		var lock 0
		set_task_var 1966 4 32 @lock 1
	endif
	
	
endproc 