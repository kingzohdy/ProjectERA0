; 珍妮


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b永远不要相信自己的眼睛，它会欺骗你。
	
		task_add_option 2122 1 100
		task_add_option 2123 1 101
		task_add_option 2124 1 102
		task_add_option 2133 1 103
		task_add_option 2134 1 104
		
	
	talk
endproc

PROC 100
	task_do_option 2122 1
ENDPROC
PROC 101
	task_do_option 2123 1
ENDPROC
PROC 102
	task_do_option 2124 1
ENDPROC
PROC 103
	task_do_option 2133 1
ENDPROC
PROC 104
	task_do_option 2134 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2124
		task_fini 2124 1
	endif
endproc






