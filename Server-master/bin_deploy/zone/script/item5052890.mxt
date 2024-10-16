

proc 1
	
	strvar ret rad rad1
	DEC_ITEM_NUM_INPAK 5052890 1 ret
	if ret = 0
		rand rad 10
		if rad = 0
			add_item_num_inpak 5043219 1 1 0
		endif
		if rad > 0 and rad < 10
			rand rad1 3
			if rad1 = 0
				add_item_num_inpak 5043189 1 1 0
			endif
			if rad1 = 1
				add_item_num_inpak 5043199 1 1 0
			endif
			if rad1 = 2
				add_item_num_inpak 5043209 1 1 0
			endif
		
		endif
	endif
endproc  