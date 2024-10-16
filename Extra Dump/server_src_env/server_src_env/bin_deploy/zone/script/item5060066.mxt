;Éú´æ½±ÕÂ¡Á1

proc 1
	strvar ret
	
	ADD_MONEY 0 10 78 ret
	if ret = 0
		dec_item_num_inpak 5060066 1 ret
	
	endif
endproc 
