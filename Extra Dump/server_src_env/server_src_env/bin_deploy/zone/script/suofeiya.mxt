

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 在残酷的战争中，平凡普通的生活是多么的可贵啊。
	
	task_add_option 4044 1 101
	task_add_option 4045 1 102
	task_add_option 4046 1 103
	task_add_option 4047 1 104
	task_add_option 4048 1 105
	task_add_option 4049 1 106
	task_add_option 4055 1 107
	task_add_option 4056 1 108
	task_add_option 4063 1 109
	task_add_option 4065 1 110
	task_add_option 4066 1 111
	task_add_option 4067 1 112
	task_add_option 4068 1 113
	task_add_option 4071 1 114
	task_add_option 4200 1 115
	task_add_option 4203 1 116
	task_add_option 4204 1 117
	task_add_option 4205 1 118
	task_add_option 4210 1 119
	talk
endproc 


PROC 101
	task_do_option 4044 1
ENDPROC
PROC 102
	task_do_option 4045 1
ENDPROC
PROC 103
	task_do_option 4046 1
ENDPROC
PROC 104
	task_do_option 4047 1
ENDPROC
PROC 105
	task_do_option 4048 1
ENDPROC
PROC 106
	task_do_option 4049 1
ENDPROC
PROC 107
	task_do_option 4055 1
ENDPROC
PROC 108
	task_do_option 4056 1
ENDPROC
PROC 109
	task_do_option 4063 1
ENDPROC
PROC 110
	task_do_option 4065 1
ENDPROC
PROC 111
	task_do_option 4066 1
ENDPROC
PROC 112
	task_do_option 4067 1
ENDPROC
PROC 113
	task_do_option 4068 1
ENDPROC
PROC 114
	task_do_option 4071 1
ENDPROC
PROC 115
	task_do_option 4200 1
ENDPROC
PROC 116
	task_do_option 4203 1
ENDPROC
PROC 117
	task_do_option 4204 1
ENDPROC
PROC 118
	task_do_option 4205 1
ENDPROC
PROC 119
	task_do_option 4210 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4046
		task_fini 4046 1
	endif 
	if 2 = 4047
		task_fini 4047 1
	endif 
	if 2 = 4049
		task_fini 4049 1
	endif
	if 2 = 4063
		task_fini 4063 1
	endif
	if 2 = 4068
		task_fini 4068 1
	endif
	if 2 = 4200
		task_fini 4200 1
	endif
endproc
