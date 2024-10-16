

proc 6
	strvar M pX pY Mx My Map Mid1
	GET_PLAYER_MAPINFO 0 Map 0 pX pY
;	get_task_var 1966 0 32 1 Mid1
	get_role_var 45 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx My	
	
	
	GET_POINT_DIST 6707 22577 @Mx @My M
	if M < 500 and map = 2300
		task_fini 4453 1
		get_role_var 45 32 1 Mid1
		DEL_ID_MON @Mid1 1
		set_role_var 45 32 0 1
	endif
endproc 