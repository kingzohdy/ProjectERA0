;½ðÇ®°ü60~80



proc 1
	
	strvar money Ret
	
	rand money 801
	add money 1600
	
	ADD_MONEY 0 @money 21 Ret
	if Ret = 0
		dec_item_num_inpak 5052817 1 0
	endif
endproc

