

proc 1
	
	strvar ret rad rad1
	DEC_ITEM_NUM_INPAK 5052887 1 ret
	if ret = 0
		rand rad 10
		if rad = 0
			add_item_num_inpak 5043216 1 1 0
		endif
		if rad > 0 and rad < 10
			rand rad1 3
			if rad1 = 0
				add_item_num_inpak 5043186 1 1 0
			endif
			if rad1 = 1
				add_item_num_inpak 5043196 1 1 0
			endif
			if rad1 = 2
				add_item_num_inpak 5043206 1 1 0
			endif
		
		endif
	endif
endproc  