

proc 1
	

	NEW_MON 207005 1 12789 12747 0 0 0 0 0 
	
	add_sys_msg_start
	add_sys_msg 向前走，消灭你看见的所有怪物，你就可以离开这里了！
	set_sys_msg_mode 3 0 0
	map_msg_info
	
	strvar loc
	var loc 0
	set_pworld_var 0 8 @loc 1

	strvar locmon
	var locmon 0
	set_pworld_var 1 8 @locmon 1
	
	strvar locq
	var locq 0
	set_pworld_var 2 8 @locq 1
	
	;任务状态
	strvar locmonum
	var locmonum 0
	set_pworld_var 3 8 @locmonum 1
	
	;只提示一次
	strvar locf
	var locf 0
	set_pworld_var 4 8 @locf 1
	;
	strvar time
	var time 0
	set_pworld_var 5 8 @time 1
	
endproc 

proc 3

	;怪物第一次死亡的时候 只提示一次
	get_pworld_var 4 8 1 locf 
	if locf = 10
		get_pworld_var 5 8 1 time
		add time 1
		set_pworld_var 5 8 @time 1
		if time = 3
			add_sys_msg_start
			add_sys_msg 忽然格罗佛的声音响起：别停手，继续杀！驱龙马上就会来帮你！
			set_sys_msg_mode 3 0 0
			map_msg_info
		
			var locf 20
			set_pworld_var 4 8 @locf 1
		endif
	endif

	strvar Num
	get_map_mon_num 0 207005 1 Num 1
	get_pworld_var 0 8 1 loc
	if loc = 0
		if Num >= 8
			;怪物达到8只可以刷机甲兽
			get_pworld_var 1 8 1 locmon
			var locmon 10
			set_pworld_var 1 8 @locmon 1
			
			;锁 只进一次
			var loc 1
			set_pworld_var 0 8 @loc 1
		endif
	endif
		
	strvar locmonum
	if Num >= 30
		;任务失败
		get_pworld_var 3 8 1 locmonum 
		var locmonum 1
		set_pworld_var 3 8 @locmonum 1 
		
		
	endif
	if Num = 0
		get_pworld_var 3 8 1 locmonum 
		var locmonum 2
		set_pworld_var 3 8 @locmonum 1 
		
	endif
endproc  