




proc 1


	DEC_ITEM_NUM_INPAK 5012115 1 100
	if 100 = 0
		strvar Mid
	;	GET_TARGET_INFO 1 1 Mid
		GET_PLAYER_ID 1 Mid
		EFFECT_ID 0 @Mid playMontion(%d,"352115@reset")
	;	effect 0 0 playEffct(13000,37000,"effect\\entity\\352115.ent")
		
	endif
endproc 

