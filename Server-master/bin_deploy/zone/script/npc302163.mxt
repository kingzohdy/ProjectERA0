;npc提尔利

proc 1
	task_stat 4447 1 20
	 if 20 = 2
		 task_fini 4447 1
	 endif
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b啊哈，你就是大名鼎鼎的$_player$，听说，你是天空岛推选出来最优秀的士兵，是未来的英雄呢！我长大了也要像你一样！
    add_talk #b#b你愿意跟我说说你的故事吗？

	
		
		TASK_ADD_OPTION 4485 0 100
		TASK_ADD_OPTION 4481 0 101
		TASK_ADD_OPTION 4483 0 102
		
		TASKCYCLE_ADD_OPTION 138 1 120
	talk
endproc 

proc 100
	TASK_DO_OPTION 4485 0
endproc
proc 101
	TASK_DO_OPTION 4481 0
endproc
proc 102
	TASK_DO_OPTION 4483 0
endproc
PROC 120	
	TASKCYCLE_DO_OPTION 138 1
ENDPROC
