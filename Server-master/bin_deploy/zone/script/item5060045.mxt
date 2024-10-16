
proc 1
	GET_PACK_SPACE 0 30
	if 30 = 0
		 add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif

	GET_ITEM_NUM_INPAK 5060041 1 100
	if 100 > 0 
 

		dec_item_num_inpak 5060045 1 10
		dec_item_num_inpak 5060041 1 20
		
		if 10 = 0 and 20 = 0
			GET_PLAYER_BASE_ATT 1 1 20 1
			if 20 < 60
				add_item_num_inpak 5230043 1 1 0
			endif
			if 20 >= 60 and 20 < 70
				add_item_num_inpak 5230044 1 1 0
			endif
			if 20 >= 70 and 20 <= 120
				add_item_num_inpak 5230045 1 1 0
			endif
		endif
	 else
		add_sys_msg_start
		add_sys_msg 你背包中没有粽叶，不能合成吉祥粽
		set_sys_msg_mode 3 0 0
		msg_info

	endif
endproc 