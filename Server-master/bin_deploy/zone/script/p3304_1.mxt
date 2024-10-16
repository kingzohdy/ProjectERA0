;ÊØ»¤Ö®¼û


proc 2
	strvar R Ret
	task_stat 2054 R Ret
	if R = 0 and Ret = 2
		strvar Mid
		GET_PLAYER_ID 1 Mid
		TARGET_ADD_STATUS 1 @Mid 3014 1 3 0
	endif

endproc 