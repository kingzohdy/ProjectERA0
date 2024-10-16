;大地之心邀请函


proc 1
	strvar Ret
	TASK_STAT 3341 1 Ret
	if Ret = 2
	
		task_fini 3341 1
	endif
endproc 