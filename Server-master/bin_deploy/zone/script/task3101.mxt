

proc 6
	strvar map
	GET_PLAYER_MAPINFO 1 map 1 1 1 
	if map = 40100 or map = 40200 or map = 40300 or map = 40400 or map = 40500
		task_fini 3101 1
	endif
	if map = 40600 or map = 40700 or map = 40800 or map = 40900 or map = 42100 or map = 42200 
		task_fini 3101 1	
	endif
endproc 