

proc 6
	strvar M pX pY Mx My Map Mid1
	GET_PLAYER_MAPINFO 0 Map 0 pX pY
;	get_task_var 1966 0 32 1 Mid1
	get_role_var 45 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx My	
	
	
	GET_POINT_DIST 15861 9965 @Mx @My M
	
	if map = 2100 or map = 2101 or map = 2102 or map = 2103 or map = 2105 or map = 2106 or map = 2107 or map = 2108
		if M < 500
			task_fini 4451 1
			get_role_var 45 32 1 Mid1
			DEL_ID_MON @Mid1 1
			set_role_var 45 32 0 1
		endif
	endif
endproc 