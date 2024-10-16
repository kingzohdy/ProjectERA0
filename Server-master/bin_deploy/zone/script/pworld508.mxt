;;;;∑® ¶ ‘¡∂

proc 1
	var 34 0
	set_pworld_var 7 8 @34 1
	
	var 30 0
	set_pworld_var 0 32 @30 1
endproc 


proc 3
	strvar Num
	GET_PWORLD_ROLE_NUM Num 1
	if Num = 0
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
	endif


endproc 