
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b生命可以是甜的，也可以是苦的，但不能是枯燥无味的。你可以胜利，也可以失败，但你不能屈服。
		
	
		TASKCYCLE_ADD_OPTION 109 1 140
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 109 1
endproc

