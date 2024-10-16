




proc 1


	DEC_ITEM_NUM_INPAK 5012113 1 100
	if 100 = 0
		strvar Mid
		GET_TARGET_INFO 1 1 Mid
		EFFECT_ID 0 @Mid playMontion(%d,"352120@reset")
		
	endif
endproc 

