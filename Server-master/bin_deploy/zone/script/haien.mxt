proc 1
	dialog 1
	add_talk 再没有援兵的话，我们之前的努力完全毫无意义，铜鼓村一定会遭受野蛮人的进攻。
	task_add_option 1060 1 101
	task_add_option 1061 1 102
	task_add_option 1062 1 103
	task_add_option 1063 1 104
	task_add_option 1069 1 105
	task_add_option 1070 1 106
	task_add_option 1071 1 107
	task_add_option 1072 1 108
	task_add_option 1073 1 109
	task_add_option 1075 1 110
	task_add_option 1076 1 111
	task_add_option 1077 1 112
	
	task_add_option 4027 1 113
	task_add_option 4029 1 114
	task_add_option 4034 1 115
	task_add_option 4035 1 116
	talk
endproc

proc 101
    task_do_option 1060 1
endproc

proc 102
    task_do_option 1061 1
endproc
proc 103
    task_do_option 1062 1
endproc
proc 104
    task_do_option 1063 1
endproc
proc 105
    task_do_option 1069 1
endproc
proc 106
    task_do_option 1070 1
endproc
proc 107
    task_do_option 1071 1
endproc
proc 108
    task_do_option 1072 1
endproc
proc 109
    task_do_option 1073 1
endproc
proc 110
    task_do_option 1075 1
endproc
proc 111
    task_do_option 1076 1
endproc
proc 112
    task_do_option 1077 1
endproc
proc 113
    task_do_option 4027 1
endproc
proc 114
    task_do_option 4029 1
endproc

proc 115
    task_do_option 4034 1
endproc
proc 116
    task_do_option 4035 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1071
		task_fini 1071 1
	endif
	if 2 = 1077
		task_fini 1077 1
	endif
endproc
