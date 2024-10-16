; 皮克希

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我每天都在问自己，做为一个人我该做些什么，我是不是要站起来抗争，向所有人证明我的价值，而这是不是意味着我的一生就将耗费在这满是硝烟的世界？
	
		task_add_option 2111 1 100
		task_add_option 2112 1 101
		task_add_option 2113 1 102
		task_add_option 2118 1 103
		task_add_option 2119 1 104

	
	talk
endproc

PROC 100
	task_do_option 2111 1
ENDPROC
PROC 101
	task_do_option 2112 1
ENDPROC
PROC 102
	task_do_option 2113 1
ENDPROC
PROC 103
	task_do_option 2118 1
ENDPROC
PROC 104
	task_do_option 2119 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2113
		task_fini 2113 1
	endif

endproc





