;召集村民

proc 1
	strvar Mid1 val Mid2 Mid3 Mid4 Loc Tim
	var Mid1 0
	set_task_var 1544 0 32 @Mid1 1
	var Mid2 0
	set_task_var 1544 4 32 @Mid2 1
	var Mid3 0
	set_task_var 1544 8 32 @Mid3 1
	var Mid4 0
	set_task_var 1544 12 32 @Mid4 1
	
	var val 0
	set_task_var 1544 16 8 @val 1
	var Loc 0
	set_task_var 1544 17 8 @Loc 1
	var Tim 0
	set_task_var 1544 18 8 @Tim 1
endproc 
proc 5
	
	get_task_var 1544 0 32 1 Mid1
	get_task_var 1544 4 32 1 Mid2
	get_task_var 1544 8 32 1 Mid3
	DEL_ID_MON @Mid1 1
	DEL_ID_MON @Mid2 1
	DEL_ID_MON @Mid3 1
	
	
endproc 

proc 6
	strvar M1 Mx1 My1  Map
	var M1 500
	var Mx1 0
	var My1 0
	
	GET_PLAYER_MAPINFO 0 Map 0 0 0
	get_task_var 1544 16 8 1 val		
	if Map != 3303 and val != 0 
		var val 0
		set_task_var 1544 16 8 @val 1
		
		var Loc 0
		set_task_var 1544 17 8 @Loc 1
	endif
	
	get_task_var 1544 0 32 1 Mid1
	GET_ID_MON_INFO @Mid1 1 1 1 1 1 Mx1 My1	
	GET_POINT_DIST 9000 3600 @Mx1 @My1 M1
	
	if M1 < 500 
		task_fini 1544 1
		get_task_var 1544 0 32 1 Mid1
		get_task_var 1544 4 32 1 Mid2
		get_task_var 1544 8 32 1 Mid3
		DEL_ID_MON @Mid1 1
		DEL_ID_MON @Mid2 1
		DEL_ID_MON @Mid3 1
	endif

	get_task_var 1544 17 8 1 Loc
	if Loc = 10
		add Tim 1
		set_task_var 1544 18 8 @Tim 1
		if Tim = 10
			add_sys_msg_start
			add_sys_msg 有一名村民响应了你的号召，快将他带回去见艾斯尤特吧！
			set_sys_msg_mode 3 0 0
			msg_info	
			var Tim 0
			set_task_var 1544 18 8 @Tim 1
			var Loc 0
			set_task_var 1544 17 8 @Loc 1
		endif
	endif
	
endproc 