;Éú´æ½±ÕÂ¡Á1

proc 1
	strvar ret
	
	ADD_MONEY 0 20 78 ret
	if ret = 0
		dec_item_num_inpak 5060067 1 ret
	
	endif
endproc 
