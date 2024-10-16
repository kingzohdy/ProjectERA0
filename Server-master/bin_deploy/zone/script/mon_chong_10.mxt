proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	

	if Mid = 240001
		NEW_MON 233029 1 @px @py 0 0 0 0 0 
	endif


	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	if Mid = 206371
		NEW_MON 206392 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 206372
		NEW_MON 206393 1 @px @py 0 0 0 0 0 
	endif

	
endproc 








