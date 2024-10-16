proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py
	
	if Mid = 235008
		NEW_MON 206357 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 231026
		NEW_MON 206358 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 241005
		NEW_MON 206359 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 200022
		NEW_MON 206360 1 @px @py 0 0 0 0 0 
	endif
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	if Mid = 206357
		NEW_MON 206378 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 206358
		NEW_MON 206379 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 206359
		NEW_MON 206380 1 @px @py 0 0 0 0 0 
	endif
	if Mid = 206360
		NEW_MON 206381 1 @px @py 0 0 0 0 0 
	endif
	
	
endproc 








