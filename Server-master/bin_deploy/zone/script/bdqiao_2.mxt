


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b君子之心，可大可小；丈夫之志，能屈能伸。
		task_add_option 1362 1 100
		task_add_option 1363 1 101
		task_add_option 1364 1 102
		task_add_option 1365 1 103
		task_add_option 1366 1 104
		task_add_option 1358 1 105
		task_add_option 1359 1 106
		task_add_option 1360 1 107
;		task_add_option 1376 1 108

	
	talk
	
endproc 

PROC 100
	task_do_option 1362 1
ENDPROC
PROC 101
	task_do_option 1363 1
ENDPROC
PROC 102
	task_do_option 1364 1
ENDPROC
PROC 103
	task_do_option 1365 1
ENDPROC
PROC 104
	task_do_option 1366 1
ENDPROC

PROC 105
	task_do_option 1358 1
ENDPROC
PROC 106
	task_do_option 1359 1
ENDPROC
PROC 107
	task_do_option 1360 1
ENDPROC
;PROC 108
;	task_do_option 1376 1
;ENDPROC
proc 2

	get_trig_task 1 2
	if 2 = 1366
		task_fini 1366 1
	endif
	if 2 = 1360
		task_fini 1360 1
	endif
endproc 
	
	