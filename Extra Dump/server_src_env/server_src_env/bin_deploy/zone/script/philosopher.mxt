proc 1
	;get_role_var 0 2 11 10
	dialog 1
	add_talk 文明的力量，蕴含在每个信仰它，愿意去保护它的人身上。

	task_add_option 1063 1 101
	task_add_option 1064 1 102
	task_add_option 1068 1 103
	task_add_option 1071 1 104
	task_add_option 1072 1 105
	task_add_option 1078 1 106
	task_add_option 1079 1 107
	task_add_option 1080 1 108
	task_add_option 1081 1 109
	
	task_add_option 4024 1 110
	talk
endproc

proc 101
    task_do_option 1063 1
endproc
proc 102
    task_do_option 1064 1
endproc
proc 103
    task_do_option 1068 1
endproc
proc 104
    task_do_option 1071 1
endproc
proc 105
    task_do_option 1072 1
endproc
proc 106
    task_do_option 1078 1
endproc
proc 107
    task_do_option 1079 1
endproc
proc 108
    task_do_option 1080 1
endproc
proc 109
    task_do_option 1081 1
endproc
proc 110
    task_do_option 4024 1
endproc


proc 2
	get_trig_task 1 2
	if 2 = 1064
		task_fini 1064 1
	endif
	if 2 = 1072
		task_fini 1072 1
	endif
	if 2 = 1081
		task_fini 1081 1
	endif
	if 2 = 1080
		task_fini 1080 1
	endif
endproc

