



proc 1
	strvar ret rad
	dec_item_num_inpak 5052855 1 ret
	if ret = 0
		rand rad 5
		if rad = 0
			add_item_num_inpak 2718407 1 1 0
		endif
		if rad = 1
			add_item_num_inpak 2718427 1 1 0
		endif
		if rad = 2
			add_item_num_inpak 2718447 1 1 0
		endif
		if rad = 3
			add_item_num_inpak 2718467 1 1 0
		endif
		if rad = 4
			add_item_num_inpak 2718487 1 1 0
		endif
	endif
endproc 