;302201	格奥尔基


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b若放任黑暗议会在此扎地生根，以后的形势将日趋严峻。
	add_talk #b#b#Name#请随时待命，战争将随时一触即发。
	
	task_add_option 3104 1 102
	
	TASK_STAT 3104 10 11
	if @10 = 0 and @11 = 2
		task_fini 3104 500
	endif
	
	talk
endproc 

proc 102
    task_do_option 3104 1
endproc

