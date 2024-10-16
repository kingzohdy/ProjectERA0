;;;;«π ÷ ‘¡∂


proc 3

	strvar Num
	GET_PWORLD_ROLE_NUM Num 1
	if Num = 0
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
	endif

endproc 