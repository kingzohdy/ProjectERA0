
;亚历山大
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b阿拜多斯的血色追随者无穷无尽，如果得不到援助的话，我们恐怕……
;	task_add_option 1560 1 101
	task_add_option 1561 1 102
	task_add_option 1562 1 103
	
	task_add_option 1567 1 104
	task_add_option 1568 1 105
	task_add_option 1569 1 106
	task_add_option 1580 1 107
	task_add_option 1581 1 108
	
	task_add_option 1586 1 109
	task_add_option 1587 1 110
	task_add_option 1588 1 111


	talk
endproc 


PROC 102
	task_do_option 1561 1
ENDPROC
PROC 103
	task_do_option 1562 1
ENDPROC
PROC 104
	task_do_option 1567 1
ENDPROC
PROC 105
	task_do_option 1568 1
ENDPROC
PROC 106
	task_do_option 1569 1
ENDPROC
PROC 107
	task_do_option 1580 1
ENDPROC
PROC 108
	task_do_option 1581 1
ENDPROC

PROC 109
	task_do_option 1586 1
ENDPROC

PROC 110
	task_do_option 1587 1
ENDPROC

PROC 111
	task_do_option 1588 1
ENDPROC




proc 2
	get_trig_task 1 2
 
	if 2 = 1562
		task_fini 1562 1
	endif 
	if 2 = 1581
		task_fini 1581 1
	endif 
endproc
