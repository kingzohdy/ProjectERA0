proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 233006
		NEW_MON 206362 1 @px @py 0 0 0 0 0 
	endif

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	if Mid = 206362
		NEW_MON 206383 1 @px @py 0 0 0 0 0 
	endif
	
	
endproc 








