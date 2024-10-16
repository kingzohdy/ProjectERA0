
;;;;

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052113 1 10
		if 10 = 0
		;ADD_BIND_ITEM_NUM_INPAK 5054002 1 1 0
			rand 50 3
			if 50 = 0
				add_item_num_inpak 5012120 1 1 0
			endif
			if 50 = 1
				add_item_num_inpak 5012121 1 1 0
			endif
			if 50 = 2 
				add_item_num_inpak 5012122 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif


endproc  







