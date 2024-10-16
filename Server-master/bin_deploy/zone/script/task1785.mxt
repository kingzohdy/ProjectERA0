
proc 2
	STRVAR a
	TASK_STAT 1770 1 a
	if a = 2
		TASK_FINI 1770 1
			if 1 = -1
				return
			endif
	endif
endproc

