

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 莱拉姐姐说了，只要西德尼乖乖的，晚餐就会有香喷喷的烤肉。
	
	task_add_option 4050 1 101
	task_add_option 4051 1 102
	task_add_option 4052 1 103
	task_add_option 4053 1 104
	task_add_option 4060 1 105
	task_add_option 4061 1 106
	task_add_option 4062 1 107
	task_add_option 4069 1 108
	task_add_option 4070 1 109
	task_add_option 4205 1 110
	task_add_option 4206 1 111
	task_add_option 4207 1 112
	task_add_option 4208 1 113
	talk
endproc 


PROC 101
	task_do_option 4050 1
ENDPROC
PROC 102
	task_do_option 4051 1
ENDPROC
PROC 103
	task_do_option 4052 1
ENDPROC
PROC 104
	task_do_option 4053 1
ENDPROC
PROC 105
	task_do_option 4060 1
ENDPROC
PROC 106
	task_do_option 4061 1
ENDPROC
PROC 107
	task_do_option 4062 1
ENDPROC
PROC 108
	task_do_option 4069 1
ENDPROC
PROC 109
	task_do_option 4070 1
ENDPROC
PROC 110
	task_do_option 4205 1
ENDPROC
PROC 111
	task_do_option 4206 1
ENDPROC
PROC 112
	task_do_option 4207 1
ENDPROC
PROC 113
	task_do_option 4208 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 4062
		task_fini 4062 1
	endif 
	if 2 = 4053
		task_fini 4053 1
	endif 
	if 2 = 4208
		task_fini 4208 1
	endif
endproc
