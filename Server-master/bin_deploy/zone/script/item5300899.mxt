

proc 1
	strvar mid ret
	GET_PLAYER_ID 1 mid
	TARGET_ADD_STATUS 1 @mid 1018 4 120 ret
	if ret = 0
		task_fini 2119 1
	endif

endproc  