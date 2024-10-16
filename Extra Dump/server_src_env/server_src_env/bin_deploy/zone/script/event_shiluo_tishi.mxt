proc 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 69
		strvar tim
		var tim 0
		set_event_var 0 32 @tim 1
		
		add_sys_msg_start
		add_sys_msg 5分钟后失落之域第七层的大门即将关闭，第七层所有人将遣返回第六层
		set_sys_msg_mode 3 0 0
		SVR_MSG_INFO
	endif
endproc 


proc 3
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 69
		strvar tim
		get_event_var 0 32 1 tim
		add tim 1
		set_event_var 0 32 @tim 1
		
		if tim = 12
			add_sys_msg_start
			add_sys_msg 3分钟后失落之域第七层的大门即将关闭，第七层所有人将遣返回第六层
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
		if tim = 24
			add_sys_msg_start
			add_sys_msg 1分钟后失落之域第七层的大门即将关闭，第七层所有人将遣返回第六层
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
		if tim = 30
			add_sys_msg_start
			add_sys_msg 失落之域第七层的大门已重新关闭，第七层所有人将遣返回第六层
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
	endif
endproc 
