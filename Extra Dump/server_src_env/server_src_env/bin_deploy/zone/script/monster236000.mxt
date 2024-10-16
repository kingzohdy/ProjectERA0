;腐皮豺狼
proc 1
	var 10 0
	set_mon_var 0 8 @10 1

endproc

proc 2

	TASK_STAT 1432 1 20
	if 20 = 2 

	endif
endproc
proc 4
		GET_MON_VAR 0 8 1 10
		add 10 1
		set_mon_var 0 8 @10 1
	
		if 10 = 19

				add_sys_msg_start
				add_sys_msg 有老鼠从你手中逃脱，留下一个轻蔑的背景，仿佛在说：伙计，不给力啊！
				set_sys_msg_mode 1 0 0
				MON_OWNER_MSG_INFO
				
				DESTROY_MON 30	

		endif
endproc


