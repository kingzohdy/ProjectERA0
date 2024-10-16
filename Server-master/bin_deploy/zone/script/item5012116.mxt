




proc 1


	DEC_ITEM_NUM_INPAK 5012116 1 100
	if 100 = 0
		strvar Mid
	;	GET_TARGET_INFO 1 1 Mid
		GET_PLAYER_ID 1 Mid
	;	EFFECT_ID 0 @Mid playMontion(%d,"352116@reset")
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 0 playEffct($@x$,$@y$,0,"effect\\entity\\352116.ent",15)

	endif
endproc 

