;5WÒø±Ò

proc 1
	
	strvar money Ret
	
	ADD_MONEY 0 50000 20 Ret
	if Ret = 0
		dec_item_num_inpak 5052956 1 0
	endif
	
endproc
