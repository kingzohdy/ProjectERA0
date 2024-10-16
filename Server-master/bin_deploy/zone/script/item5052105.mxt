;Ã°ÏÕ²¹¸øÏä

proc 1
	strvar Lv Ret Num
	GET_PACK_SPACE 0 Num
	if Num >= 2
		
		DEC_ITEM_NUM_INPAK 5052105 1 Ret
		if Ret = 0
			GET_ID_PLAYER_INFO 0 8 Lv
			if Lv < 20
				ADD_ITEM_NUM_INPAK 5010000 20 1 0
				ADD_ITEM_NUM_INPAK 5011000 20 1 0
			endif
			if Lv >= 20 and Lv < 40
				ADD_ITEM_NUM_INPAK 5010001 20 1 0
				ADD_ITEM_NUM_INPAK 5011001 20 1 0
			endif
			if Lv >= 40 and Lv < 60
				ADD_ITEM_NUM_INPAK 5010002 20 1 0
				ADD_ITEM_NUM_INPAK 5011002 20 1 0
			endif
			if Lv >= 60 and Lv < 80
				ADD_ITEM_NUM_INPAK 5010003 20 1 0
				ADD_ITEM_NUM_INPAK 5011003 20 1 0
			endif
			if Lv >= 80 and Lv < 100
				ADD_ITEM_NUM_INPAK 5010004 20 1 0
				ADD_ITEM_NUM_INPAK 5011004 20 1 0
			endif
			if Lv >= 100 and Lv <= 120
				ADD_ITEM_NUM_INPAK 5010005 20 1 0
				ADD_ITEM_NUM_INPAK 5011005 20 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
		set_sys_msg_mode 1 0 0
		msg_info	
	
	endif
endproc 