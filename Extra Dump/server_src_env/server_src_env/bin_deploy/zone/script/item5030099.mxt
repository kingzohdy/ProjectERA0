;Ã»ÌùÓÊÆ±µÄĞÅ


proc 1
	strvar Ret
	TASK_STAT 3000 1 Ret
	if Ret = 2
	
		task_fini 3000 1
	endif
endproc 