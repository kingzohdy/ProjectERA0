

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk @@@@@@@@@@@@@@@@@@@@@@@@@@@
	
;	task_add_option 1082 1 101

	talk
endproc 

PROC 101
;	task_do_option 1082 1
ENDPROC



proc 2
;	get_trig_task 1 2
;	if 2 = 1090
;		task_fini 1090 1
;	endif

endproc
