

proc 1
	strvar lv
	task_stat 3003 0 50

	get_player_base_att 1 2 3 4
	if 3 >= 18 
		if 50 = 3 
			transmit 4400 23725 27202 1
		else
			add_sys_msg_start
			add_sys_msg 需要完成宠物系列任务才能进入！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 实在抱歉,您的等级未达到18级,请您过段时间再来吧...
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc 

