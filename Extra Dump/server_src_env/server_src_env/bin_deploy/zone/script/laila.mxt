

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 上帝啊，保佑我们度过这个残酷的战争吧。
	
	task_add_option 1104 1 101
	task_add_option 1105 1 102
	task_add_option 1106 1 103
	task_add_option 1114 1 104
	task_add_option 1115 1 105
	task_add_option 1116 1 106
	task_add_option 1121 1 107
	task_add_option 1200 1 108
	task_add_option 1201 1 109
	task_add_option 1205 1 110
	task_add_option 1212 1 111
	task_add_option 1213 1 112
	task_add_option 1214 1 113
	task_add_option 4208 1 114
	task_add_option 4209 1 115
	talk
endproc 

PROC 101
	task_do_option 1104 1
ENDPROC
PROC 102
	task_do_option 1105 1
ENDPROC
PROC 103
	task_do_option 1106 1
ENDPROC
PROC 104
	task_do_option 1114 1
ENDPROC
PROC 105
	task_do_option 1115 1
ENDPROC
PROC 106
	task_do_option 1116 1
ENDPROC
PROC 107
	task_do_option 1121 1
ENDPROC
PROC 108
	task_do_option 1200 1
ENDPROC
PROC 109
	task_do_option 1201 1
ENDPROC
PROC 110
	task_do_option 1205 1
ENDPROC
PROC 111
	task_do_option 1212 1
ENDPROC
PROC 112
	task_do_option 1213 1
ENDPROC
PROC 113
	task_do_option 1214 1
ENDPROC
PROC 114
	task_do_option 4208 1
ENDPROC
PROC 115
	task_do_option 4209 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1106
		task_fini 1106 1
	endif
	if 2 = 1116
		task_fini 1116 1
	endif 
	if 2 = 1200
		task_fini 1200 1
	endif 
	;if 2 = 1113
	;	task_fini 1113 1
	;endif
endproc
