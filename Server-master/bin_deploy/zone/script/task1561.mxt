
;É¾³ýÐÅÍ½

proc 5

	strvar Mid1
	get_task_var 1561 0 32 1 Mid1
	DEL_ID_MON @Mid1 1
	
endproc 

proc 6
	strvar X Y M Mx My Map
	GET_PLAYER_MAPINFO 0 Map 0 X Y
	if Map != 3302
		
		set_task_var 1561 4 32 0 1
	endif
	
	get_task_var 1561 0 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx My	
	
	GET_POINT_DIST @Mx @My 4200 7400 M
	if M < 500
	
		strvar Mid1
		task_fini 1561 1
		get_task_var 1561 0 32 1 Mid1
		DEL_ID_MON @Mid1 1
	endif

	
	
	
	
	
endproc
