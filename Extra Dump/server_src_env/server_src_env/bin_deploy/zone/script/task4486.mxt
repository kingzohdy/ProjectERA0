


proc 8

	strvar map x y m
	GET_PLAYER_MAPINFO 1 map 1 x y
	GET_POINT_DIST @x @y 27800 30800 m
	if m < 500

		strvar MotIon
		ID_PLAYER_MOTION 0 0 MotIon    
		if MotIon = 10709
			
			task_fini 4486 1
		endif 
		
	endif
endproc 


