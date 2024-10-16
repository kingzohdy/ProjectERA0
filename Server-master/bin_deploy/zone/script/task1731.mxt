
proc 1
	STRVAR m1 m2 m3 id
	var m1 0
	set_task_var 1731 0 8 @m1 1
	var m2 0
	set_task_var 1731 1 8 @m2 1
	var m3 0
	set_task_var 1731 2 8 @m3 1
endproc

proc 6
	GET_MAP_NPC_ID 303029 20 1
	GET_MAP_NPC_ID 301001 21 1
	GET_MAP_NPC_ID 302022 22 1
	get_task_var 1731 0 8 1 m1
	get_task_var 1731 1 8 1 m2
	get_task_var 1731 2 8 1 m3	
	GET_PLAYER_ID 1 id
	if @m1 = 1  
		mon_speech @20 2 0 1 这肉的味道，世间已没有任何语言可以形容它！		
			if 2 = 0
				var @m1 2	
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	if @m2 = 1 
		mon_speech @21 2 0 1 鹿肉，好吃啊！我从来没吃过这么好吃的鹿肉！
			if 2 = 0
				var @m2 2
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	if @m3 = 1 
		mon_speech @22 2 0 1 汁镇在肉里，好似江河汇聚，入口即化！哇！这鹿肉太棒了！		
			if 2 = 0
				var @m3 2
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	;if @m1 = 2 and @m2 = 2 and @m3 = 2
	;	task_fini 1731 0
	;endif
	set_task_var 1731 0 8 @m1 1
	set_task_var 1731 1 8 @m2 1
	set_task_var 1731 2 8 @m3 1
endproc

