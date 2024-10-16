


proc 1

	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b无论如何，我们都要拿下这座监狱的控制权！无论如何，我们都不能放任黑暗议会在我们的领土上肆意妄为！
		task_add_option 1953 1 100
		task_add_option 1954 1 101
		task_add_option 1955 1 102
		task_add_option 1956 1 103
		task_add_option 1957 1 104
		task_add_option 1960 1 105
	talk
	
endproc


proc 100
	task_do_option 1953 1
endproc 
proc 101
	task_do_option 1954 1
endproc 
proc 102
	task_do_option 1955 1
endproc 
proc 103
	task_do_option 1956 1
endproc 
proc 104
	task_do_option 1957 1
endproc 
proc 105
	task_do_option 1960 1
endproc 


proc 2
	get_trig_task 1 2
	if 2 = 1957
		task_fini 1957 1
	endif
	if 2 = 1960
		task_fini 1960 1
	endif
endproc
