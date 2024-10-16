


proc 2
	strvar Hp Mid PlyMid
	
	get_mon_var 0 32 1 PlyMid 
	GET_PLAYER_ID 1 Mid
;	GET_MON_INFO 1 1 Mid Hp 1 1 1 1
;	GET_MON_HP_PERCENT Mid Hp
	if @Mid != @PlyMid

		TASK_FAIL 1532 1
	endif
endproc 
