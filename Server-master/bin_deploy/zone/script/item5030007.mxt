

proc 1
	GET_NOW_TIME 0 0 0 20 30 40 50
	if 30 >= 16 and 30 < 24  
	else
	
		add_sys_msg_start
		add_sys_msg 当前资源采集场尚未开放,开放时间为每天16:00-23:59
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
proc 2
	GET_NOW_TIME 0 0 0 20 30 40 50
	if 30 >= 16 and 30 < 24  
	else
	
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 当前资源采集场尚未开放,开放时间为每天16:00-23:59
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 
