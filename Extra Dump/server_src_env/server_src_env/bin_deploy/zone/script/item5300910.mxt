
;���׽ⶾ���
proc 1
	strvar ret
	task_stat 2137 1 ret
	if ret = 2
		strvar rad
		rand rad 2
		if rad = 0
			DESTROY_MON 1
		;	del_mon 1
			RUN_TASK_PROGRESS 0 2137 1 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG �з�Ӧ�ˣ��з�Ӧ�ˣ��з�Ӧ�ˡ���
			set_sys_msg_mode 3 0 0
			MSG_INFO	
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���������ҩ��û���κη�Ӧ
			set_sys_msg_mode 3 0 0
			MSG_INFO	
		
		endif
	endif
endproc  


