;Éú´æ½±ÕÂ¡Á1

proc 1
	strvar ret
	
	ADD_MONEY 0 1 78 ret
	if ret = 0
		dec_item_num_inpak 5060063 1 ret
	
	endif
endproc 
