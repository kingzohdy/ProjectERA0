proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 218001
		NEW_MON 206363 1 @px @py 0 0 0 0 0 
	endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	if Mid = 206363
		NEW_MON 206384 1 @px @py 0 0 0 0 0 
	endif
	
	
endproc 








