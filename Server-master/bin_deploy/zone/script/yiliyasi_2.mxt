

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我从来不主张战争，它却是通往和平的必经之路！
	
	task_add_option 1528 1 101
	task_add_option 1521 1 102
	task_add_option 1529 1 103
	task_add_option 1530 1 104
	task_add_option 1532 1 105
	task_add_option 1533 1 106

	talk
endproc 

PROC 101
	task_do_option 1528 1
ENDPROC
PROC 102
	task_do_option 1521 1
ENDPROC
PROC 103
	task_do_option 1529 1
ENDPROC
PROC 104
	task_do_option 1530 1
ENDPROC
PROC 105
	task_do_option 1532 1
ENDPROC
PROC 106
	task_do_option 1533 1
ENDPROC


proc 2
	get_trig_task 1 2

	if 2 = 1533
		task_fini 1533 1
	endif
endproc
