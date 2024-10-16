proc 1
	get_role_var 0 2 11 10
	dialog 1
	add_talk 铜鼓村的局势越来越动荡了，被野蛮部落包围的处境如何才能打破。
	task_add_option 1053 1 101	
	task_add_option 1054 1 102
	task_add_option 1055 1 103
	task_add_option 1064 1 104
	task_add_option 1065 1 105
	
	task_add_option 4022 1 106
	task_add_option 4023 1 107	
	task_add_option 4027 1 108
	task_add_option 4028 1 109
	task_add_option 4031 1 110
	task_add_option 4037 1 111
	task_add_option 4038 1 112
	task_add_option 4039 1 113
	
	option 200 #进入时间城
	if 10 = 1
		option 100 #来嘛
	endif
	talk
endproc
proc 101
    task_do_option 1053 1
endproc
proc 102
    task_do_option 1054 1
endproc
proc 103
    task_do_option 1055 1
endproc
proc 104
    task_do_option 1064 1
endproc
proc 105
    task_do_option 1065 1
endproc
proc 106
    task_do_option 4022 1
endproc
proc 107
    task_do_option 4023 1
endproc
proc 108
    task_do_option 4027 1
endproc
proc 109
    task_do_option 4028 1
endproc
proc 110
    task_do_option 4031 1
endproc
proc 111
    task_do_option 4037 1
endproc
proc 112
    task_do_option 4038 1
endproc
proc 113
    task_do_option 4039 1
endproc

proc 100
	set_role_var 0 2 0 10
	set_role_var 0 3 1 10
	
	;add_sys_msg_start
	;add_sys_msg BINGO!
	;set_sys_msg_mode 4 0 0
	;msg_info
	
	dialog 0
	add_talk SB!
	talk
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1054
		task_fini 1054 1
	endif
	if 2 = 1065
		task_fini 1065 1
	endif
	if 2 = 4022
		task_fini 4022 1
	endif
	if 2 = 4031
		task_fini 4031 1
	endif
	if 2 = 4038
		task_fini 4038 1
	endif
	if 2 = 4039
		task_fini 4039 1
	endif
endproc

proc 200
transmit 1000 28174 11219 0
endproc
