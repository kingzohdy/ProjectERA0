proc 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 59
		strvar tim
		var tim 0
		set_event_var 0 32 @tim 1
		
		add_sys_msg_start
		add_sys_msg 时间城上空凝聚了一团雾霾，10分钟后【黑暗物质】将疯狂袭来！
		set_sys_msg_mode 0 0 0
		SVR_MSG_INFO
	endif
endproc 


proc 3
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 59
		get_event_var 0 32 1 tim
		add tim 1
		set_event_var 0 32 @tim 1
		
		if tim = 30
			
			add_sys_msg_start
			add_sys_msg 时间城上空凝聚了一团雾霾，5分钟后【黑暗物质】将疯狂袭来！
			set_sys_msg_mode 0 0 0
			SVR_MSG_INFO
		endif
	endif

	
endproc 








