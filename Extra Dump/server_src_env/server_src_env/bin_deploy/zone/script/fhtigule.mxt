;伊米

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b<提古勒昏昏沉沉的，嘴里不停地念叨着什么，你凑近才隐约听到>
	add_talk #b#b瓶……瓶盖……
		task_add_option 1918 1 100
		task_add_option 1919 1 101

	
	talk
endproc

PROC 100
	task_do_option 1918 1
ENDPROC
PROC 101
	task_do_option 1919 1
ENDPROC
;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
