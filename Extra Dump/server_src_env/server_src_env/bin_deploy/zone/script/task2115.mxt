
;·ÉÐÐ1


proc 6
;	strvar Ret
;	task_stat 2115 1 Ret	
;	if Ret = 2
;		strvar x y m
;		GET_PLAYER_MAPINFO 0 0 0 x y
;		GET_POINT_DIST @x @y 33600 18500 m
;		if m <= 500
;		
;			task_fini 2115 1 
;		endif
;	endif
endproc 

