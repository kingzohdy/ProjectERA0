

proc 1
	strvar num1 num2
	GET_MAP_MON_NUM 3216 201350 1 num1 1 
	if num1 = 0
		NEW_MAP_MON 201350 1 3216 6414 6405 0 0 0 45 0 0 0
	endif
	

	GET_MAP_MON_NUM 3213 201332 1 num2 1 
	if num2 = 0
	
		NEW_MAP_MON 201332 1 3213 7181 6392 0 0 0 45 0 0 0
	endif
endproc 