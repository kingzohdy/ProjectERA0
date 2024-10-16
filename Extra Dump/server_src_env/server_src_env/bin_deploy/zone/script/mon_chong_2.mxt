proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py
	

	if Mid = 200022
		NEW_MON 206360 1 @px @py 0 0 0 0 0 
	endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	if Mid = 206360
		NEW_MON 206381 1 @px @py 0 0 0 0 0 
	endif
	
	
endproc 








