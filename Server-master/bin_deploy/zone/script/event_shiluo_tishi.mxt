proc 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 69
		strvar tim
		var tim 0
		set_event_var 0 32 @tim 1
		
		add_sys_msg_start
		add_sys_msg 5���Ӻ�ʧ��֮����߲�Ĵ��ż����رգ����߲������˽�ǲ���ص�����
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
			add_sys_msg 3���Ӻ�ʧ��֮����߲�Ĵ��ż����رգ����߲������˽�ǲ���ص�����
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
		if tim = 24
			add_sys_msg_start
			add_sys_msg 1���Ӻ�ʧ��֮����߲�Ĵ��ż����رգ����߲������˽�ǲ���ص�����
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
		if tim = 30
			add_sys_msg_start
			add_sys_msg ʧ��֮����߲�Ĵ��������¹رգ����߲������˽�ǲ���ص�����
			set_sys_msg_mode 3 0 0
			SVR_MSG_INFO
		endif
	endif
endproc 
