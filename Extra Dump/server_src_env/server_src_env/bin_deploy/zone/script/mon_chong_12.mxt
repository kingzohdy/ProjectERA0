proc 1

	strvar Mid px py
	GET_MON_INFO 10 Mid 1 1 1 1 px py

	if Mid = 254014
		NEW_MON 206422 1 @px @py 0 0 0 0 0 
	endif
	
endproc
