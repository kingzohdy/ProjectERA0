;½ðÇ®°ü0~20



proc 1
	
	strvar money Ret
	
	rand money 161
	add money 320
	
	ADD_MONEY 0 @money 21 Ret
	if Ret = 0
		dec_item_num_inpak 5052814 1 0
	endif
endproc

