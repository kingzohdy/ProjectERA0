

proc 1
	strvar ret
	dec_item_num_inpak 5052110 1 ret
	if ret = 0
;		ADD_MONEY 0 100 21 0
		rand 50 100
		if 50 < 30
			ADD_ITEM_NUM_INPAK 5050651 1 1 0
		endif
		if 50 >= 30 and 50 < 60
			
			ADD_ITEM_NUM_INPAK 5050652 1 1 0
		endif
		if 50 >= 60 and 50 < 100
		
			ADD_ITEM_NUM_INPAK 5060037 1 1 0
		endif
		
	endif
endproc 
