
;小方
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b世人都说科技生产难，不难能叫科技生产吗？世人都觉得见习科技最容易，却无奈巧妇难为无米之炊。
	add_talk #b#b每周一，我都会出现在这里，回收科技材料，希望你也能每周都来捐献材料，别以为捐献是很吃亏的事情，你会因此而提升科技熟练度，并且还会获得一个百宝箱。至于其中有什么宝贝，开了才知道。

		task_add_option 30016 15 100
		task_add_option 30017 15 101
		task_add_option 30018 15 102
		
		TASKCYCLE_ADD_OPTION 131 1 120
	talk
endproc



PROC 100
	task_do_option 30016 1
ENDPROC
PROC 101
	task_do_option 30017 1
ENDPROC
PROC 102
	task_do_option 30018 1
ENDPROC

PROC 120	
	TASKCYCLE_DO_OPTION 131 1
ENDPROC









