
proc 1

	STRVAR x2 y2 a
endproc

proc 6
		get_player_mapinfo 0 1 2 x2 y2
		get_point_dist @x2 @y2 7000 12800 a
		if 2 = 3500
					if a <= 500
					  task_fini 1742 1	
					endif
		endif
endproc






