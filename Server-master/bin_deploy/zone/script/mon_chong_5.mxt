proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 233015
		NEW_MON 233028 1 @px @py 0 0 0 0 0 
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	if Mid = 233028
		NEW_MON 233030 1 @px @py 0 0 0 0 0 
	endif

	
endproc 








