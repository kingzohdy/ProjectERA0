
;ª§ÀÕ…Ò√ÿ“Ø√«

proc 1
	strvar Mid
	var Mid 0 
	set_task_var 2121 0 32 @Mid 1

endproc 
proc 5
	
	get_task_var 2121 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc 

proc 6
	strvar Ret
	task_stat 2121 1 Ret	
	if Ret = 2
		strvar x y m
		;GET_PLAYER_MAPINFO 0 0 0 x y
	
		get_task_var 2121 0 32 1 Mid 
		GET_ID_MON_INFO @Mid 1 0 0 0 0 x y
		GET_POINT_DIST @x @y 32800 32000 m
		if m <= 1500
			task_fini 2121 1 
			DEL_ID_MON @Mid 1
		endif
	endif
endproc 

proc 4
	
	get_task_var 2121 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc
