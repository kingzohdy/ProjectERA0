;勇士岛1
proc 1
	strvar Loc Tim 
	var Loc 0
	set_task_var 3406 0 8 @Loc 1
	var Tim 0
	set_task_var 3406 1 8 @Tim 1
endproc

proc 2
	strvar Mid
	GET_MAP_NPC_ID 306021 Mid 1
	mon_speech @Mid 2 0 1 #Name#取得了成功，埃里斯倒下了！


endproc

proc 6
	strvar Mid
	GET_MAP_NPC_ID 306021 Mid 1
	
	get_task_var 3406 0 8 1 Loc
	if Loc = 1
	
		get_task_var 3406 1 8 1 Tim
		add Tim 1
		set_task_var 3406 1 8 @Tim 1
		if Tim =  1
			mon_speech @Mid 2 0 1 常胜将军埃里斯上场！
		endif
		if Tim = 5
			mon_speech @Mid 2 0 1 比赛开始，请选手各就各位！

		endif
		if Tim = 1
		;	mon_speech @Mid 2 0 1 比赛正式开始！
		;	NEW_MON 282023 1 12400 13000 0 0 0 0 0
			NEW_LIFE_MON 282023 1 12400 13000 0 0 0 0 1800 0 0
			var Tim 0
			set_task_var 3406 1 8 @Tim 1
			var Loc 0
			set_task_var 3406 0 8 @Loc 1
			
			strvar Loc1 Loc2 Loc3
			get_map_val 0 8 1 Loc1
			var Loc1 0
			set_map_val 0 8 @Loc1 1
			get_map_val 1 8 1 Loc2
			var Loc2 0
			set_map_val 1 8 @Loc2 1
			get_map_val 2 8 1 Loc3
			var Loc3 0
			set_map_val 2 8 @Loc3 1
		endif
		
	endif

endproc




