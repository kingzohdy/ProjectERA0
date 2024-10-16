
proc 1

	; dec_item_num_inpak 5301005 1 0
	; add_item_num_inpak 5301007 1 1 0
	
	strvar id
	GET_TARGET_INFO 2 1 id
	if id != -1
		TARGET_ADD_STATUS 2 id 2100 1 60 1
	endif
endproc 