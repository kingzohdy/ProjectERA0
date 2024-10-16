;
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	
	if Num = 0
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
	
		DEC_ITEM_NUM_INPAK 5052857 1 100
		if 100 = -1
			return
		endif
		strvar Rad
		rand Rad 5
		switch Rad
			case 0
				ADD_ITEM_NUM_INPAK 2718107 1 1 0
			endcase
			case 1 
				ADD_ITEM_NUM_INPAK 2718127 1 1 0
			endcase
			case 2
				ADD_ITEM_NUM_INPAK 2718147 1 1 0
			endcase
			case 3 
				ADD_ITEM_NUM_INPAK 2718167 1 1 0
			endcase
			case 4 
				ADD_ITEM_NUM_INPAK 2718187 1 1 0
			endcase
		endswitch
	endif
endproc
