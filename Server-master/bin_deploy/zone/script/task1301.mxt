;;;;;;;;

proc 7
	strvar pX pY M
	GET_PLAYER_MAPINFO 1 1 1 pX pY
	GET_POINT_DIST @pX @pY 30500 48500 M

	if M < 10
		
		transmit 2100 4200 4000 1
	endif
endproc 