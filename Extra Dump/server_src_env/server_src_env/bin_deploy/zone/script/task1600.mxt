


proc 6
	strvar map
	GET_PLAYER_MAPINFO 1 map 1 1 1 
	if map = 40000 or map = 41000 or map = 41100 or map = 41200 or map = 41300
		task_fini 1600 1
	endif
	if map = 41400 or map = 41500 or map = 41600 or map = 41700 or map = 41800 or map = 41900 
		task_fini 1600 1	
	endif
endproc 