

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我知道我做的是危险的事情，也许下一秒就会失去生命，但是我不在乎！为了暖泉镇！
	task_add_option 1512 1 101
	task_add_option 1513 1 102
	task_add_option 1514 1 103
	task_add_option 1515 1 104

	task_add_option 1520 1 105
;	task_add_option 1521 1 106
	task_add_option 1522 1 107


	talk
endproc 


PROC 101
	task_do_option 1512 1
ENDPROC
PROC 102
	task_do_option 1513 1
ENDPROC
PROC 103
	task_do_option 1514 1
ENDPROC
PROC 104
	task_do_option 1515 1
ENDPROC
PROC 105
	task_do_option 1520 1
ENDPROC
;PROC 106
;	task_do_option 1521 1
;ENDPROC
PROC 107
	task_do_option 1522 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1515
		task_fini 1515 1
	endif 
	if 2 = 1522
		task_fini 1522 1
	endif 


endproc