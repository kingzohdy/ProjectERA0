

proc 4
	
	strvar x y
	ronglu_is_full 10 20
	if 10 = 0 and 20 = 1
	;	DESTROY_MON 1
	;	GET_MON_INFO 1 1 1 1 1 1 x y
	;	CALL_MON 1 293402 0 600 @x @y 1

		RONGLU_CALL_COLLECT 1 293402 600
	endif
endproc 