

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 坚守着即将沦陷的故土，支撑我们的，就是那份信念。
	
	task_add_option 1098 1 101
	task_add_option 1099 1 102
	task_add_option 1100 1 103
	task_add_option 1102 1 104
	task_add_option 1103 1 105
	task_add_option 1104 1 106
	task_add_option 1106 1 107
	task_add_option 1107 1 108
	task_add_option 1109 1 109
	task_add_option 1110 1 110
	task_add_option 1111 1 111
	task_add_option 1112 1 112
	task_add_option 1116 1 113
	task_add_option 1117 1 114
	task_add_option 1120 1 115
	task_add_option 1121 1 116
	task_add_option 1203 1 117
	task_add_option 1206 1 118
	task_add_option 1207 1 119
	task_add_option 1209 1 120
	task_add_option 1214 1 121
	talk
endproc 

PROC 101
	task_do_option 1098 1
ENDPROC
PROC 102
	task_do_option 1099 1
ENDPROC
PROC 103
	task_do_option 1100 1
ENDPROC
PROC 104
	task_do_option 1102 1
ENDPROC
PROC 105
	task_do_option 1103 1
ENDPROC
PROC 106
	task_do_option 1104 1
ENDPROC
PROC 107
	task_do_option 1106 1
ENDPROC
PROC 108
	task_do_option 1107 1
ENDPROC
PROC 109
	task_do_option 1109 1
ENDPROC
PROC 110
	task_do_option 1110 1
ENDPROC
PROC 111
	task_do_option 1111 1
ENDPROC
PROC 112
	task_do_option 1112 1
ENDPROC
PROC 113
	task_do_option 1116 1
ENDPROC
PROC 114
	task_do_option 1117 1
ENDPROC
PROC 115
	task_do_option 1120 1
ENDPROC
PROC 116
	task_do_option 1121 1
ENDPROC
PROC 117
	task_do_option 1203 1
ENDPROC
PROC 118
	task_do_option 1206 1
ENDPROC
PROC 119
	task_do_option 1207 1
ENDPROC
PROC 120
	task_do_option 1209 1
ENDPROC
PROC 121
	task_do_option 1214 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1100
		task_fini 1100 1
	endif 
	if 2 = 1104
		task_fini 1104 1
	endif 
	if 2 = 1107
		task_fini 1107 1
	endif
	if 2 = 1110
		task_fini 1110 1
	endif
	if 2 = 1117
		task_fini 1117 1
	endif
	if 2 = 1121
		task_fini 1121 1
	endif
	if 2 = 1203
		task_fini 1203 1
	endif
endproc
