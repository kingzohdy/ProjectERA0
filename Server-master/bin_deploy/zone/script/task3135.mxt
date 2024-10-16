
proc 1
	strvar PlayerX PlayerY Distance
	
	var 10 0
	set_task_var 3135 7 32 @10 1
endproc 
proc 5
		GET_PLAYER_ID 1 60
		TARGET_ADD_STATUS 1 @60 1213 2 1 1	
		
		get_task_var 3135 7 32 1 10
		DEL_ID_MON @10 1
endproc
proc 6
	
	get_task_var 3135 0 8 1 11
	if 11 = 1
	
		get_task_var 3135 1 32 1 10
		add 10 1
 		set_task_var 3135 1 32 @10 1
		
		if 10 = 5
			add_sys_msg_start
			add_sys_msg 比赛开始了，你必须取得胜利！！！
			set_sys_msg_mode 3 0 0
			msg_info	
			effect 2 0 playEffct(30700,44800,"effect\\entity\\9569_16.ent")
			GET_PLAYER_ID 1 60
			TARGET_ADD_STATUS 1 @60 1213 2 35 1	
			TARGET_ADD_STATUS 1 @60 3099 1 0 1	
			
		
			AUTO_MOVE 1 1000 20900 44900 0 45 3081 1 3600
		endif
		if 10 = 11
			add_sys_msg_start
			add_sys_msg 比赛进行中，请勿偏离赛道，胜利就在前方！
			set_sys_msg_mode 3 0 0
			msg_info
			var 10 6
			set_task_var 3135 1 32 @10 1
		endif
	
		GET_PLAYER_MAPINFO 1 2 3 PlayerX PlayerY
		GET_POINT_DIST @PlayerX @PlayerY 20900 44900 Distance		

		if Distance < 1000
			effect 2 0 playEffct(20900,44900,"effect\\entity\\9572_14.ent")	
		endif
		if Distance < 300

				GET_PLAYER_ID 1 60
				TARGET_ADD_STATUS 1 @60 1213 2 0 1	
				
				task_fini 3135 1
	
		endif
;		if 10 = 23
;			TASK_FAIL 3135 1
;			add_sys_msg_start
;			add_sys_msg 你没能追上赛马员，任务失败了！
;			set_sys_msg_mode 3 0 0
;			msg_info
;		endif
		
	endif
endproc 