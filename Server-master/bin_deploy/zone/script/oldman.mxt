proc 1
;	task_stat 1074 74 700
;	if 700 = 2
;		task_fini 1074 74
;	endif
	dialog 1
	add_talk 不起眼的角色，才能潜伏在敌人的身旁不被发现。
	task_add_option 1056 1 101
	task_add_option 1057 1 102
	task_add_option 1058 1 103
	task_add_option 1059 1 104
	task_add_option 1060 1 105

	
	
	talk
endproc

proc 101
    task_do_option 1056 1
endproc

proc 102
    task_do_option 1057 1
endproc
proc 103
    task_do_option 1058 1
endproc
proc 104
    task_do_option 1059 1
endproc
proc 105
    task_do_option 1060 1
endproc


proc 2
	get_trig_task 1 600
	if 600 = 1060
		task_fini 1060 1
	endif
endproc
