

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b哀求永远无法获得尊严，想活着，想有尊严的活着，你就必须要拿起武器，对抗那些危害世界、毁灭你尊严的人事物。
	add_talk #b#b当然，你也可以等待，等别人来拯救你，如果每个人都和你一样的想法，那么你等来的只是无止境的黑暗与毁灭，为什么你不愿意做那名拯救世界的人呢？
	add_talk #b#b如果我们每个人都起来反抗，那小小的血色追随者又算得了什么？
	
	
	task_add_option 1540 1 101
	task_add_option 1541 1 102
	task_add_option 1542 1 103
	task_add_option 1543 1 104
	task_add_option 1544 1 105
	task_add_option 1549 1 106
	task_add_option 1550 1 107
	task_add_option 1551 1 108
	task_add_option 1552 1 109



	talk
endproc 


PROC 101
	task_do_option 1540 1
ENDPROC
PROC 102
	task_do_option 1541 1
ENDPROC
PROC 103
	task_do_option 1542 1
ENDPROC
PROC 104
	task_do_option 1543 1
ENDPROC
PROC 105
	task_do_option 1544 1
ENDPROC
PROC 106
	task_do_option 1549 1
ENDPROC
PROC 107
	task_do_option 1550 1
ENDPROC
PROC 108
	task_do_option 1551 1
ENDPROC
PROC 109
	task_do_option 1552 1
ENDPROC


proc 2
	get_trig_task 1 2
 
	if 2 = 1543
		task_fini 1543 1
	endif 
	if 2 = 1550
		task_fini 1550 1
	endif 
endproc
