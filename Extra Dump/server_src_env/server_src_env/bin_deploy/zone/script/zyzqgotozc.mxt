proc 1

		transmit 1000 23800 27400 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 0 0 0
		if Map = 50500	
			RESET_PWORLD 505
		endif
		if Map = 50600	
			RESET_PWORLD 506
		endif
		if Map = 50700	
			RESET_PWORLD 507
		endif
		if Map = 50700	
			RESET_PWORLD 508
		endif
endproc 