proc 1
	dialog 1
	add_talk 在被野蛮人包围的铜鼓村，太多的战斗变成维持局面的努力。
	task_add_option 1051 1 101
	task_add_option 1052 1 102
	task_add_option 1053 1 103   
	task_add_option 1055 1 104
	task_add_option 1056 1 105   
	task_add_option 1065 1 106
	task_add_option 1066 1 107
	task_add_option 1077 1 108
	task_add_option 1078 1 109
	
	task_add_option 4030 1 110

	talk
endproc

proc 101
    task_do_option 1051 1
endproc
proc 102
    task_do_option 1052 1
endproc
proc 103
    task_do_option 1053 1
endproc
proc 104
    task_do_option 1055 1
endproc
proc 105
    task_do_option 1056 1
endproc
proc 106
    task_do_option 1065 1
endproc
proc 107
    task_do_option 1066 1
endproc
proc 108
    task_do_option 1077 1
endproc
proc 109
    task_do_option 1078 1
endproc
proc 110
    task_do_option 4030 1
endproc

proc 2
	get_trig_task 1 600
	if 600 = 1053
		task_fini 1053 1 
	endif 
	if 600 = 1056
		task_fini 1056 1
	endif
	if 600 = 1066
		task_fini 1066 1
	endif
	if 600 = 4030
		task_fini 4030 1
	endif
		if 600 = 1078
		task_fini 1078 1
	endif

endproc 
