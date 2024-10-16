

proc 1
	strvar ret
	dec_item_num_inpak 5060008 1 ret
	if ret = 0
		ADD_MONEY 0 1 77 0
	
	endif
endproc 
