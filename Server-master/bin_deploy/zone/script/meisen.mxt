

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 我们一定可以赶走疯狂的怪物，守卫住我们的故土。
	
	task_add_option 1100 1 101
	task_add_option 1101 1 102
	task_add_option 1107 1 103
	task_add_option 1108 1 104
	task_add_option 1113 1 105
	task_add_option 1114 1 106
	task_add_option 1118 1 107
	task_add_option 1119 1 108
	task_add_option 1120 1 109
	task_add_option 1206 1 110
	task_add_option 4060 1 111
	task_add_option 4062 1 112
	talk
endproc 


PROC 101
	task_do_option 1100 1
ENDPROC
PROC 102
	task_do_option 1101 1
ENDPROC
PROC 103
	task_do_option 1107 1
ENDPROC
PROC 104
	task_do_option 1108 1
ENDPROC
PROC 105
	task_do_option 1113 1
ENDPROC
PROC 106
	task_do_option 1114 1
ENDPROC
PROC 107
	task_do_option 1118 1
ENDPROC
PROC 108
	task_do_option 1119 1
ENDPROC
PROC 109
	task_do_option 1120 1
ENDPROC
PROC 110
	task_do_option 1206 1
ENDPROC
PROC 111
	task_do_option 4060 1
ENDPROC
PROC 112
	task_do_option 4062 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 1108
		task_fini 1108 1
	endif 
	if 2 = 1114
		task_fini 1114 1
	endif 
	if 2 = 1206
		task_fini 1206 1
	endif 
endproc
