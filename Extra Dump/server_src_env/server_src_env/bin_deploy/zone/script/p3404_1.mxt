;调查沙岭墓群 古老的玉片

proc 1


	strvar Ret Re
	TASK_STAT 2002 Re Ret
	if Re = 0 and Ret = 2
		strvar X Y
		GET_PLAYER_MAPINFO 1 1 1 X Y
		NEW_MAP_MON 293202 1 3404 @X @Y 500 0 0 0 20 0 0

	endif
	
endproc  