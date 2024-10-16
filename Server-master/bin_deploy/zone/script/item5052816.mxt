;½ðÇ®°ü40~60



proc 1
	
	strvar money Ret
	
	rand money 401
	add money 800
	
	ADD_MONEY 0 @money 21 Ret
	if Ret = 0
		dec_item_num_inpak 5052816 1 0
	endif
endproc

