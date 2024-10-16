
proc 1
	strvar Loc
	var Loc 0
	set_task_var 3169 0 8 @Loc 1

	strvar Time
	var Time 0
	set_task_var 3169 1 8 @Time 1
	
endproc 


proc 6

	get_task_var 3169 0 8 1 Loc
	if Loc = 1
		
		get_task_var 3169 1 8 1 Time
		add Time 1
		set_task_var 3169 1 8 @Time 1
		if Time = 2
			add_sys_msg_start
			add_sys_msg 封印已解除，影子即将降临于竞技场！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
		if Time = 4
			effect 0 0 playEffct(12500,13000,"effect\\entity\\9600_16.ent")
			NEW_MON 205052 1 12500 13000 0 0 0 0 0
			
			var Loc 0
			set_task_var 3169 0 8 @Loc 1
			var Time 0
			set_task_var 3169 1 8 @Time 1
			
			strvar LocMap
			get_map_val 10 8 1 LocMap
			var LocMap 0
			set_map_val 10 8 @LocMap 1
		endif
	
	endif
endproc 