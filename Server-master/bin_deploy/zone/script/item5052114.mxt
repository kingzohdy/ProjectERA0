
;;;;

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052114 1 10
		if 10 = 0
		;ADD_BIND_ITEM_NUM_INPAK 5054002 1 1 0
			rand 50 8
			if 50 = 0
				add_item_num_inpak 5052320 1 1 0
			endif
			if 50 = 1
				add_item_num_inpak 5052321 1 1 0
			endif
			if 50 = 2 
				add_item_num_inpak 5052322 1 1 0
			endif
			if 50 = 3
				add_item_num_inpak 5052323 1 1 0
			endif
			if 50 = 4
				add_item_num_inpak 5052324 1 1 0
			endif
			if 50 = 5
				add_item_num_inpak 5052325 1 1 0
			endif
			if 50 = 6
				add_item_num_inpak 5052326 1 1 0
			endif
			if 50 = 7
				add_item_num_inpak 5052327 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif


endproc  







