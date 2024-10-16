

proc 1
	strvar ret
	dec_item_num_inpak 5052915 1 ret
	if ret = 0
		ADD_BIND_ITEM_NUM_INPAK 3650017 1 1 0
	
	endif
endproc 
