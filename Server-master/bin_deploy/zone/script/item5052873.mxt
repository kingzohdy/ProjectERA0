;½ðÇ®°ü60~80



proc 1
	
	strvar money nDecReturn
	
	rand money 801
	add money 1600
	
	dec_item_num_inpak 5052873 1 nDecReturn

	if nDecReturn = 0
		ADD_MONEY 0 @money 20 1
	endif
	
endproc

