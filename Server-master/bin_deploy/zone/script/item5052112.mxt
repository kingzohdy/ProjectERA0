
;;;;

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052112 1 10
		if 10 = 0
		;ADD_BIND_ITEM_NUM_INPAK 5054002 1 1 0
			rand 50 1000
			if 50 >= 0 and 50 < 5
				add_item_num_inpak 5043240 1 1 0
			endif
			if 50 >= 5 and 50 < 10
				add_item_num_inpak 5043241 1 1 0
			endif
			if 50 >= 10 and 50 < 105
				add_item_num_inpak 5043242 1 1 0
			endif
			if 50 >= 105 and 50 < 200
				add_item_num_inpak 5043243 1 1 0
			endif
			if 50 >= 200 and 50 < 500
				add_item_num_inpak 5043244 1 1 0
			endif
			if 50 >= 500 and 50 < 1000
				add_item_num_inpak 5043245 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc  







