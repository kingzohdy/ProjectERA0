



proc 2
	strvar Num_1
	get_pworld_var 6 8 1 Num_1
	var Num_1 10
	set_pworld_var 6 8 @Num_1 1
	effect 0 0 playEffct(7900,7600,270,"effect\\entity\\9570_16.ent")

endproc

proc 5
	strvar Map
	GET_PLAYER_MAPINFO 0 Map 0 0 0
	if Map = 50400
		transmit 2104 4400 9600 1
	endif
	
endproc 