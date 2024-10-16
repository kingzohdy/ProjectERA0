

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 我没有梅森那么强烈的信心，对于保卫战，把我能做的做了，剩下的事情就只能交给上帝了。
	
	task_add_option 1101 1 101
	task_add_option 1102 1 102
	task_add_option 1108 1 103
	task_add_option 1109 1 104
	task_add_option 1112 1 105
	task_add_option 1113 1 106
	task_add_option 1117 1 107
	task_add_option 1118 1 108
	task_add_option 1200 1 109
	task_add_option 1202 1 110
	task_add_option 1205 1 111
	task_add_option 1208 1 112
	task_add_option 4046 1 113
	task_add_option 4047 1 114
	task_add_option 4200 1 115
	task_add_option 4201 1 116
	task_add_option 4202 1 117
	talk
endproc 


PROC 101
	task_do_option 1101 1
ENDPROC
PROC 102
	task_do_option 1102 1
ENDPROC
PROC 103
	task_do_option 1108 1
ENDPROC
PROC 104
	task_do_option 1109 1
ENDPROC
PROC 105
	task_do_option 1112 1
ENDPROC
PROC 106
	task_do_option 1113 1
ENDPROC
PROC 107
	task_do_option 1117 1
ENDPROC
PROC 108
	task_do_option 1118 1
ENDPROC
PROC 109
	task_do_option 1200 1
ENDPROC
PROC 110
	task_do_option 1202 1
ENDPROC
PROC 111
	task_do_option 1205 1
ENDPROC
PROC 112
	task_do_option 1208 1
ENDPROC

PROC 113
	task_do_option 4046 1
ENDPROC
PROC 114
	task_do_option 4047 1
ENDPROC
PROC 115
	task_do_option 4200 1
ENDPROC
PROC 116
	task_do_option 4201 1
ENDPROC
PROC 117
	task_do_option 4202 1
ENDPROC




proc 2
	get_trig_task 1 2
	if 2 = 1102
		task_fini 1102 1
	endif 
	if 2 = 1113
		task_fini 1113 1
	endif
	if 2 = 1205
		task_fini 1205 1
	endif
		if 2 = 4047
		task_fini 4047 1
	endif
	if 2 = 4202
		task_fini 4202 1
	endif
endproc
