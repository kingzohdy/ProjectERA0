


proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b有些所谓的研讨会就是请一些不三不四的人，吃一些不干不净的饭，花一些不明不白的钱，说一些不疼不痒的话，还不如真刀真枪杀敌来得痛快！
	add_talk #b#b
	add_talk #c186506注意：环组任务放弃后算作完成一次。
	add_talk #c186506注意：接取任务后，将不能使用“我很忙”将其立刻完成。#n
	
		task_add_option 1950 1 100
		task_add_option 1951 1 101
		task_add_option 1952 1 102
		task_add_option 1953 1 103
		task_add_option 1964 1 104
		
		task_add_option 1966 1 105
		task_add_option 1967 1 106
		task_add_option 1968 1 107
		task_add_option 1969 1 108
		
		TASKCYCLE_ADD_OPTION 122 1 150
		
		strvar lv
		GET_PLAYER_BASE_ATT 0 0 lv 1

		if lv >= 75 and lv < 85
			option 250 我要立刻完成【环组】牢城
		endif
		
	talk
	
endproc
proc 150
	TASKCYCLE_DO_OPTION 122 1
endproc 


PROC 100
	task_do_option 1950 1
endproc 
PROC 101
	task_do_option 1951 1
endproc 
PROC 102
	task_do_option 1952 1
endproc 
PROC 103
	task_do_option 1953 1
endproc 
PROC 104
	task_do_option 1964 1
endproc 


PROC 105
	task_do_option 1966 1
endproc 
PROC 106
	task_do_option 1967 1
endproc 
PROC 107
	task_do_option 1968 1
endproc 
PROC 108
	task_do_option 1969 1
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1953
		task_fini 1953 1
	endif

endproc
proc 250
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 75 and lv < 85
		PLAYER_BUSY_UI 20 1
	endif
endproc 

