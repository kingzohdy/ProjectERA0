

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我…我相信，人的潜力，潜力是无穷的…
	
	task_add_option 1515 1 101
	task_add_option 1516 1 102
	task_add_option 1517 1 103
	task_add_option 1518 1 104
	task_add_option 1519 1 105
	task_add_option 1520 1 106
	task_add_option 1540 1 107
	task_add_option 1550 1 108

	

	talk
endproc 


PROC 101
	task_do_option 1515 1
ENDPROC
PROC 102
	task_do_option 1516 1
ENDPROC
PROC 103
	task_do_option 1517 1
ENDPROC
PROC 104
	task_do_option 1518 1
ENDPROC
PROC 105
	task_do_option 1519 1
ENDPROC
PROC 106
	task_do_option 1520 1
ENDPROC
PROC 107
	task_do_option 1540 1
ENDPROC

PROC 108
	task_do_option 1550 1
ENDPROC



proc 2
	get_trig_task 1 2
 
	if 2 = 1520
		task_fini 1520 1
	endif 
	if 2 = 1540
		task_fini 1540 1
	endif 
endproc
