;½ðÇ®°ü80~100



proc 1
	
	strvar money Ret
	
	rand money 1201
	add money 2400
	
	ADD_MONEY 0 @money 21 Ret
	if Ret = 0
		dec_item_num_inpak 5052818 1 0
	endif
endproc

