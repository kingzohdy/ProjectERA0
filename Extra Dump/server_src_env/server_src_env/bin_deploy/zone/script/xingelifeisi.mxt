;格里菲斯
; 携带任务

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b上帝会把我们身边最好的东西拿走，以提醒我们得到的太多！
	task_add_option 1068 1 101	
;	task_add_option 1069 1 102
	task_add_option 1070 1 103
	
	task_add_option 1072 1 104
;	task_add_option 1078 1 105
;	task_add_option 1079 1 106

	talk
endproc
proc 101
    task_do_option 1068 1
endproc
proc 102
    task_do_option 1069 1
endproc
proc 103
    task_do_option 1070 1
endproc
proc 104
    task_do_option 1072 1
endproc
;proc 105
 ;   task_do_option 1078 1
;endproc
;proc 106
;    task_do_option 1079 1
;endproc

proc 2
	get_trig_task 1 2
;	if 2 = 1079 
;		task_fini 1079 1
;	endif
	if 2 = 1072
;		NEW_OWNER_TMP_ITEM 1460067 8000 30800 0 0 0 308 1095 1 3600 1
	endif
endproc









