;祭祀试炼副本


proc 1

	var 60 0
	set_pworld_var 4 32 @60 1
	
	
	
	get_pworld_var 12 32 1 70
	get_pworld_var 16 32 1 71
	get_pworld_var 20 32 1 72
	get_pworld_var 24 32 1 73
	get_pworld_var 28 32 1 74
	get_pworld_var 32 32 1 75
	

	call_mon 1 293113 0 0 13300 44000 80
	var 70 @80
	set_pworld_var 12 32 @70 1
	call_mon 1 293114 0 0 13700 44000 81
	var 71 @81
	set_pworld_var 16 32 @71 1
	call_mon 1 293115 0 0 13100 44400 82
	var 72 @82
	set_pworld_var 20 32 @72 1
	call_mon 1 293113 0 0 13900 44400 83
	var 73 @83
	set_pworld_var 24 32 @73 1
	call_mon 1 293114 0 0 13500 44800 84
	var 74 @84
	set_pworld_var 28 32 @74 1
;	call_mon 1 293115 0 0 13700 44800 85
;	var 75 @85
;	set_pworld_var 32 32 @75 1
 
 
	get_pworld_var 36 32 1 76
	var 76 0
	set_pworld_var 36 32 @76 1
	get_pworld_var 40 32 1 77
	var 77 0
	set_pworld_var 40 32 @77 1
	
endproc 


proc 3

	strvar Num
	GET_PWORLD_ROLE_NUM Num 1
	if Num = 0
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
	endif

	get_pworld_var 36 32 1 76
	if 76 = 1
		get_pworld_var 40 32 1 77
		add 77 1
		set_pworld_var 40 32 @77 1
		if 77 = 1
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在30秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 77 = 10
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在20秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 77 = 20
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在10秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif	
		if 77 = 30
			transmit 1000 23800 27400 1
;			RESET_PWORLD 507	
		
		endif
	
	endif
endproc 









