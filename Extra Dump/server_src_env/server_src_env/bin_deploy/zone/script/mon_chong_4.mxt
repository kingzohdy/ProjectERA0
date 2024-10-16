proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 240003
		NEW_MON 233029 1 @px @py 0 0 0 0 0 
	endif
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	if Mid = 233029
		NEW_MON 233031 1 @px @py 0 0 0 0 0 
	endif
	
	
	
endproc 








