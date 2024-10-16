;½ðÇ®°ü20~40



proc 1
	
	strvar money nDecReturn
	
	rand money 201
	add money 500
	
	dec_item_num_inpak 5052871 1 nDecReturn
	
	if nDecReturn = 0
		ADD_MONEY 0 @money 20 1
	endif
	
endproc

