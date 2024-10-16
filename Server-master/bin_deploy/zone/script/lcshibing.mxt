


proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b只要还活着，就有希望，我们不会放弃！不放弃自己，也不放弃消灭任何胆敢危害这个世界的渣滓
	
	strvar lock Ret
	get_task_var 1966 4 32 1 lock
	task_stat 1966 1 Ret
	if lock = 0 and Ret = 2
		option 150 #再给我一次机会吧，这次一定送你到安全地方
	endif
	
	talk
	
endproc

proc 150

	strvar lock Mid1
	get_task_var 1966 0 32 1 Mid1
	
	PLAYER_CALL_BUB 204069 0 Mid1
	set_task_var 1966 0 32 @Mid1 1
	
	var lock 1
	set_task_var 1966 4 32 @lock 1
endproc 