;½ðÇ®°ü20~40



proc 1
	
	strvar money Ret
	
	rand money 201
	add money 500
	
	ADD_MONEY 0 @money 21 Ret
	if Ret = 0
		dec_item_num_inpak 5052815 1 0
	endif
endproc

