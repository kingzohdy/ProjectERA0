proc 1
	dialog 1
	add_talk 盘山道的脆弱已经成为整个铜鼓村防线最大的软肋。
	task_add_option 4030 30 430
	talk
endproc

proc 430
    task_do_option 4030 30
endproc
