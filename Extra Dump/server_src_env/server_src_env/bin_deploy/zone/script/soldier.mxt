proc 1
	dialog 1
	add_talk 铜鼓村就在前方不远，那是个死亡盛开的地方。
	task_add_option 1051 51 151
	talk
endproc

proc 151
    task_do_option 1051 51
endproc

proc 2
	get_trig_task 500 600
	if 600 = 1051
		task_fini 1051 51
	endif 
endproc
