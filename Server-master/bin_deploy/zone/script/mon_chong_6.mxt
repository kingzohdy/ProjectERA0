proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py
	
	if Mid = 235008
		NEW_MON 206357 1 @px @py 0 0 0 0 0 
	endif

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	if Mid = 206357
		NEW_MON 206378 1 @px @py 0 0 0 0 0 
	endif

	
endproc 








