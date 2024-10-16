




proc 1

	strvar hp
	GET_TARGET_INFO 3 2 hp
	if hp != 0
		DEC_ITEM_NUM_INPAK 5012112 1 100
		if 100 = 0
	
		
		strvar Mid
		GET_TARGET_INFO 3 1 Mid
		mon_set_hp -40 @Mid 1
;		EFFECT_ID 0 @Mid playMontion(%d,"352120")
		

		;	del_mon 1
		endif
	endif
endproc 

