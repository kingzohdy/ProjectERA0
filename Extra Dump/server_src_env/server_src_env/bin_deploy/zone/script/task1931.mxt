;;;;;;;;;; …Ó»Îª¢—®



proc 6
	strvar Ret
	HAS_STATUS 1018 Ret
	if Ret = -1
		TASK_FAIL 1931 1
		
	endif
endproc
