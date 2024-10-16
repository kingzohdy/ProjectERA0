; 银助


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b像你这种人，要怎么改变呢？
	add_talk #b#b不，你无法被改变，所以你只能死！
	
		task_add_option 2124 1 100
		
	
	talk
endproc

PROC 100
	task_do_option 2124 1
ENDPROC


proc 2
;	get_trig_task 1 2
;	if 2 = 2124
;		task_fini 2124 1
;	endif
endproc






