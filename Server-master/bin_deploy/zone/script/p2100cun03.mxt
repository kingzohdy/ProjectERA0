;焚木村区域1

PROC 1

	TASK_STAT 1314 1 20	
	
	if 20 = 2 

		add_sys_msg_start
		add_sys_msg 继续向前走，进入村子，看看是否能有所发现。
		set_sys_msg_mode 3 0 0
		msg_info
		
		effect 0 0 playEffct(16350,17000,"effect\\entity\\9570_16.ent")
	endif

	
ENDPROC

