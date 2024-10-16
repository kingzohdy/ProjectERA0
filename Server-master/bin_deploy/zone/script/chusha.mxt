proc 1
	dialog 1
	add_talk 我要找到最完美的炼金药剂，最完美的力量才能保护铜鼓村。
	task_add_option 4031 1 101
	task_add_option 4032 1 102
	task_add_option 4033 1 103
	task_add_option 4035 1 104
	task_add_option 4036 1 105
	task_add_option 4037 1 106
	talk
endproc

proc 101
    task_do_option 4031 1
endproc

proc 102
    task_do_option 4032 1
endproc

proc 103
    task_do_option 4033 1
endproc
proc 104
    task_do_option 4035 1
endproc
proc 105
    task_do_option 4036 1
endproc
proc 106
    task_do_option 4037 1
endproc

proc 2
	get_trig_task 500 600
	if 600 = 4033
		task_fini 4033 33
	endif
	if 600 = 4036
		task_fini 4036 33
	endif
endproc
