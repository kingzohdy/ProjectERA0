
;嘉年华提示


proc 1

	add_sys_msg_start
	add_sys_msg 时间城的#L奈皮尔@@(1000,42300,29300)#n将在10分钟后打开“欢乐嘉年华”的大门！
	set_sys_msg_mode 6 0 0
	svr_msg_info	

endproc 


proc 3
	GET_NOW_TIME 0 0 0 20 30 40 50
	if 30 = 12 and 40 = 54 and 50 > 49
		add_sys_msg_start
		add_sys_msg 时间城的#L奈皮尔@@(1000,42300,29300)#n将在5分钟后打开“欢乐嘉年华”的大门！
		set_sys_msg_mode 6 0 0
		svr_msg_info	

	endif
	if 30 = 17 and 40 = 55 and 50 > 49
		add_sys_msg_start
		add_sys_msg 时间城的#L奈皮尔@@(1000,42300,29300)#n将在5分钟后打开“欢乐嘉年华”的大门！
		set_sys_msg_mode 6 0 0
		svr_msg_info	

	endif
endproc 
