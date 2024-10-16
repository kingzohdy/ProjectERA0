;;;; ÑÌ»¨


proc 1
	strvar Mid X Y rad1 rad2 rad3
	GET_PLAYER_MAPINFO 1 1 1 X Y
;	effect 0 0 playEffct($@X$,$@Y$,"effect\\entity\\352102.ent")
;	var rad1 0
;	var rad2 0
	
;	rand rad1 200
;	rand rad2 200
	
;	rand rad3 2
;	if rad3 = 0
;		add X @rad1
;		add Y @rad2
;	endif
;	if rad3 = 1
;		sub X @rad1
;		sub Y @rad2
;	endif

;	sub Y 100
	NEW_LIFE_MON 352111 1 @X @Y 200 0 0 0 60 10 0

endproc

