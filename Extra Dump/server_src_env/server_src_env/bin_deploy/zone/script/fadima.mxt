proc 1
	dialog 1
	
	add_talk 试练由我开启，没有勇气的人是没办法承担起文明的责任.
	
	task_add_option 1001 21 101 
	task_add_option 1002 22 102
	task_add_option 1004 24 104
	task_add_option 1005 25 105
	task_add_option 1006 26 106
	talk
endproc
 
proc 101 
	task_do_option 1001 21
endproc

proc 102
	task_do_option 1002 22
endproc

proc 104
	task_do_option 1004 24
endproc

proc 105
	task_do_option 1005 25
endproc

proc 106
	task_do_option 1006 26
endproc

proc 2
	get_trig_task 600 800
	if 800 = 1002
		task_fini 1002 21
	endif 
endproc
