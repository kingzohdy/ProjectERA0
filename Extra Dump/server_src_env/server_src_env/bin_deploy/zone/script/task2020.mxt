


proc 5
	strvar Mid
	get_task_var 2020 0 32 1 Mid 	
	DEL_ID_MON @Mid 100

endproc 
proc 6
	strvar X Y M
	GET_PLAYER_MAPINFO 0 0 0 X Y
	strvar Mid Mx My
	get_task_var 2020 0 32 1 Mid 	
	GET_ID_MON_INFO @Mid 1 1 1 1 1 Mx My	
	
	GET_POINT_DIST @Mx @My 21800 21800 M
	if M < 500
		task_fini 2020 1
		DEL_ID_MON @Mid 100
	endif	

endproc 
