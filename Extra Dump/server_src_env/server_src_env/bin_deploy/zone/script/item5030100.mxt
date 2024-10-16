;运动会参赛证


proc 1
	strvar Ret
	TASK_STAT 3131 1 Ret
	if Ret = 2
	
		task_fini 3131 1
	endif
endproc 