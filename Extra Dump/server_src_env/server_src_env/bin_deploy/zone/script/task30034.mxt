;环境任务



proc 6
	strvar ret
	task_stat 30034 1 ret
	if ret = 2
		strvar map
		GET_PLAYER_MAPINFO 0 map 0 0 0
		if map = 11500 or map = 11501 or map = 11502 or map = 11503 or map = 11504 or map = 11505 or map = 11506 or map = 11507 or map = 11508 or map = 11509
			task_fini 30034 1
		endif
	endif
endproc 

