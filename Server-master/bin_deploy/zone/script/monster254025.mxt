


proc 2
	strvar Hp Mid PlyMid
	
	get_mon_var 0 32 1 PlyMid 
	GET_PLAYER_ID 1 Mid
	if @Mid != @PlyMid

		TASK_FAIL 4612 1
	endif
endproc 