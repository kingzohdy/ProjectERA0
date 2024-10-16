proc 1
	dialog 1

	add_talk 时候不早了，海桑这死老头子什么时候才能回来呢…
	task_add_option 4027 1 101
	task_add_option 4028 1 102
	task_add_option 4029 1 103
	
	talk
	
endproc

proc 101
    task_do_option 4027 1
endproc
proc 102
    task_do_option 4028 1
endproc
proc 103
    task_do_option 4029 1
endproc
proc 2
	get_trig_task 1 2
	if 2 = 4029
		task_fini 4029 1
	endif 
endproc
