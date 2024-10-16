;½ðÇ®°ü80~100



proc 1
	
	strvar money nDecReturn
	
	rand money 1201
	add money 2400
	
	dec_item_num_inpak 5052874 1 nDecReturn
	
	if nDecReturn = 0
		ADD_MONEY 0 @money 20 1
	endif
	
endproc

