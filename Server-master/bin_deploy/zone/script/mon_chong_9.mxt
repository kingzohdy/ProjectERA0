proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 230016
		NEW_MON 206366 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 233002
		NEW_MON 206365 1 @px @py 0 0 0 0 0 
	endif
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	if Mid = 206366
		NEW_MON 206387 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 206365
		NEW_MON 206386 1 @px @py 0 0 0 0 0 
	endif

endproc 








