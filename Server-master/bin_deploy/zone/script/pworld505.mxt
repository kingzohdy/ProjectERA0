;战士试炼



proc 1

	get_pworld_var 0 32 1 30
	var 30 0
	set_pworld_var 0 32 @30 1
	
	get_pworld_var 4 32 1 31
	var 31 0
	set_pworld_var 4 32 @31 1


endproc 

proc 3
	strvar Num
	GET_PWORLD_ROLE_NUM Num 1
	if Num = 0
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
	endif

	get_pworld_var 0 32 1 30
	if 30 = 1
		get_pworld_var 4 32 1 31
		add 31 1
		set_pworld_var 4 32 @31 1
		if 31 = 1
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在30秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 31 = 10
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在20秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 31 = 20
			add_sys_msg_start
			add_sys_msg 你已经放弃任务，你将会在10秒后自动传出副本！
			set_sys_msg_mode 3 0 0
			msg_info
		endif	
		if 31 = 30
			transmit 1000 23800 27400 1
			RESET_PWORLD 505	
		
		endif
	
	endif
endproc 
