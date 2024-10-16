



proc 1
	strvar ret rad
	dec_item_num_inpak 5052856 1 ret
	if ret = 0
		rand rad 5
		if rad = 0
			add_item_num_inpak 2718409 1 1 0
		endif
		if rad = 1
			add_item_num_inpak 2718429 1 1 0
		endif
		if rad = 2
			add_item_num_inpak 2718449 1 1 0
		endif
		if rad = 3
			add_item_num_inpak 2718469 1 1 0
		endif
		if rad = 4
			add_item_num_inpak 2718489 1 1 0
		endif

	endif
endproc 