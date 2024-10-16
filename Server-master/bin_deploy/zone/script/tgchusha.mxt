proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b呵呵，我是铜鼓村的楚莎，有什么我能帮上忙的吗？
	task_add_option 1308 1 101
	task_add_option 1309 1 102
	task_add_option 1310 1 103
	task_add_option 1311 1 104
	

	talk
endproc

proc 101
    task_do_option 1308 1
endproc
proc 102
    task_do_option 1309 1
endproc

proc 103
    task_do_option 1310 1
endproc

proc 104
    task_do_option 1311 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1311
		task_fini 1311 1
	endif

endproc
