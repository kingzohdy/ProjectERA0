proc 1
	dialog 1
	get_role_var 0 3 11 10
	if 10 = 1
		option 100 #来嘛
	endif
	add_talk 有着年轻的心，就算再老也不会去担心。
	task_add_option 4025 1 101
	task_add_option 4026 1 102

	talk
	
endproc

proc 101
    task_do_option 4025 23
endproc

proc 102
    task_do_option 4026 26
endproc

proc 100
	set_role_var 0 3 0 10
	
	;add_sys_msg_start
	;add_sys_msg BINGO!
	;set_sys_msg_mode 4 0 0
	;msg_info
	
	task_fini 1078 78
	
	dialog 0
	add_talk SB!
	talk
	
endproc
