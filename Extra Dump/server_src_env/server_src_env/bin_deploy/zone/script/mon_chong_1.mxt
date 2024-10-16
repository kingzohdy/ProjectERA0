proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py
	

	if Mid = 232006
		NEW_MON 206361 1 @px @py 0 0 0 0 0 
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	if Mid = 206361
		NEW_MON 206382 1 @px @py 0 0 0 0 0 
	endif
	
	
endproc 








