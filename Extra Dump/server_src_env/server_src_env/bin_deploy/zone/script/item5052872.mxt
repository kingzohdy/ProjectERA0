;½ðÇ®°ü40~60



proc 1
	
	strvar money nDecReturn
	
	rand money 401
	add money 800
	
	dec_item_num_inpak 5052872 1 nDecReturn

	if nDecReturn = 0
		ADD_MONEY 0 @money 20 1
	endif
	
endproc

