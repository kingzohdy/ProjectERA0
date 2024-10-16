

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你仔细观察了很久，终于发现一只纤细瘦弱的手，那是一只女人的手，虽然已经体无完肤，但仍能从某些细节上分辨出来。
	
	task_add_option 1412 1 101
	task_add_option 1413 1 102
	talk
endproc 
PROC 101
	task_do_option 1412 1
ENDPROC
PROC 102
	task_do_option 1413 1
ENDPROC

proc 2
	get_trig_task 1 2

	if 2 = 1413
		task_fini 1413 1
	endif

endproc
