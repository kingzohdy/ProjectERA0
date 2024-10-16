

proc 6
	strvar M pX pY Mx My Map Mid1
	GET_PLAYER_MAPINFO 0 Map 0 pX pY
;	get_task_var 1966 0 32 1 Mid1
	get_role_var 45 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx My	
	
	
	GET_POINT_DIST 16414 16404 @Mx @My M
	if map = 2000 or map = 2001 or map = 2002 or map = 2003 or map = 2004 or map = 2005 or map = 2006 or map = 2007
		if M < 500 
			task_fini 4450 1
			get_role_var 45 32 1 Mid1
			DEL_ID_MON @Mid1 1
			set_role_var 45 32 0 1
		endif
	endif
endproc 