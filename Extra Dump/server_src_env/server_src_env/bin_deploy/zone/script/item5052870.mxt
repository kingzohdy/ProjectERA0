;½ðÇ®°ü0~20



proc 1
	
	strvar money nDecReturn
	
	rand money 161
	add money 320
	
	dec_item_num_inpak 5052870 1 nDecReturn
	
	if nDecReturn = 0
		ADD_MONEY 0 @money 20 1
	endif
		
endproc

