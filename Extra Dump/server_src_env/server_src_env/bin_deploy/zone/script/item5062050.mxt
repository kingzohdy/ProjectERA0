;±¦Í¼¾í


proc 1
	strvar rand1
	rand rand1 3
	if rand1 = 0
		ADD_ITEM_NUM_INPAK 5062000 1 1 12
	endif
	
	if rand1 = 1
		ADD_ITEM_NUM_INPAK 5062001 1 1 12
	endif
	
	if rand1 = 2
		ADD_ITEM_NUM_INPAK 5062002 1 1 12
	endif
	
	IF 12 = 0
		DEC_ITEM_NUM_INPAK 5062050 1 1
	Else
		add_sys_msg_start
		add_sys_msg ±³°üÒÑÂú£¬ÇëÔ¤Áô1¸ö¿ÕÎ»¡£
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF

endproc		