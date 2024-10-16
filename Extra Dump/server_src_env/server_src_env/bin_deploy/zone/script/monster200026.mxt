
;镖车招怪


proc 1
	strvar Mid Rad
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	TARGET_ADD_STATUS 3 @Mid 1262 1 5 1
	
	rand Rad 3
	if Rad = 0
		MON_SPEECH @Mid 1 1 1 发现目标，启动掠夺模式
	endif
	if Rad = 1
		MON_SPEECH @Mid 1 1 1 目标已进入计划范围，启动掠夺模式
	endif
	if Rad = 2
		MON_SPEECH @Mid 1 1 1 确认目标无误，启动掠夺模式
	endif
endproc


