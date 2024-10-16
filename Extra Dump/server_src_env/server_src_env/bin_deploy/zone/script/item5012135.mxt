


proc 1
	strvar x y
	GET_PLAYER_MAPINFO 1 1 1 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\352117.ent")
endproc  

