; 盖拉得

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b天地间有许多景象是要闭了眼才看得见的，譬如梦。
	
		task_add_option 1934 1 100
		task_add_option 1935 1 101

	talk
endproc

PROC 100
	task_do_option 1934 1
ENDPROC
PROC 101
	task_do_option 1935 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
