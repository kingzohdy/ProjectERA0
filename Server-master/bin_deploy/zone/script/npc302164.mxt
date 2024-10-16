;npc布兰尼

proc 1

	task_stat 4448 1 20
	 if 20 = 2
		 task_fini 4448 1
	 endif
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我好久没有这么开心过了，有这么多哥哥姐姐陪着我，好希望每天都是儿童节呢。

	
		TASK_ADD_OPTION 4486 0 100
		TASK_ADD_OPTION 4482 0 101
		TASK_ADD_OPTION 4484 0 102
		
		TASKCYCLE_ADD_OPTION 139 1 120
	talk
endproc 

proc 100
	TASK_DO_OPTION 4486 0
endproc
proc 101
	TASK_DO_OPTION 4482 0
endproc
proc 102
	TASK_DO_OPTION 4484 0
endproc

PROC 120	
	TASKCYCLE_DO_OPTION 139 1
ENDPROC

