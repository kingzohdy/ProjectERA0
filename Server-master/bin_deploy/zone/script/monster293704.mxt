
proc 7		
	strvar PosX PosY
	GET_MON_INFO 10 0 0 0 0 0 PosX PosY
	if @10 = 0
		SUB_PLAYER_HP 0 -1000 1
		DESTROY_MON 1
		PLAYER_COLLECT_BREAK
		NEW_LIFE_MON 212109 1 @PosX @PosY 0 0 0 0 0 0 0
	endif
endproc

