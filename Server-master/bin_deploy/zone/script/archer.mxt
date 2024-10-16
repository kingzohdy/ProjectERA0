proc 1
	dialog 1
	add_talk 丛林中的王者，是借助别人的血来成就的。
	task_add_option 1066 1 101
	task_add_option 1067 1 102
	task_add_option 1069 1 103
	task_add_option 1073 1 104
	task_add_option 1074 1 105
	task_add_option 1075 1 106
	
	task_add_option 4033 1 107
	task_add_option 4034 1 108

	OPTION 201 #盘山道
	talk
endproc

proc 101
    task_do_option 1066 1
endproc
proc 102
    task_do_option 1067 1
endproc
proc 103
    task_do_option 1069 1
endproc
proc 104
    task_do_option 1073 1
endproc
proc 105
    task_do_option 1074 1
endproc
proc 106
    task_do_option 1075 1
endproc
proc 107
    task_do_option 4033 1
endproc
proc 108
    task_do_option 4034 1
endproc

proc 201
    ENTER_PWORLD 101 0 1
endproc
proc 2
	get_trig_task 1 600
	if 600 = 4034
		task_fini 4034 1
	endif
endproc
