
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我们这个世界，从不会给一个悲观、自怨自艾者颁发奖牌。只会幻想而不行动的人，永远也体会不到功成名就时的喜悦。
	add_talk #b#b
	add_talk #c186506注意：环组任务放弃后算作完成一次。
	add_talk #c186506注意：接取任务后，将不能使用“我很忙”将其立刻完成。#n

		task_add_option 1865 1 100
		task_add_option 1866 1 101
		TASKCYCLE_ADD_OPTION 109 1 140
		
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 65 and lv < 75
		option 240 我要立刻完成【环组】普罗圣地
	endif
	
	talk
	
endproc 
PROC 100
	task_do_option 1865 1
ENDPROC
PROC 101
	task_do_option 1866 1
ENDPROC

proc 140
	TASKCYCLE_DO_OPTION 109 1
endproc
proc 200
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 240
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc






