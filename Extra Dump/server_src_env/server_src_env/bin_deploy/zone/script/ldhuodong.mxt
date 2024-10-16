proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk Áúµ°¹È»î¶¯½Ì¹Ù
	
	task_add_option 2007 1 101
	task_add_option 2008 1 102

	task_add_option 4207 1 103
	talk
endproc 

PROC 101
	task_do_option 2007 1
ENDPROC
PROC 102
	task_do_option 2008 1
ENDPROC
PROC 103
	task_do_option 4207 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2008 
		task_fini 2008 1
	endif

endproc