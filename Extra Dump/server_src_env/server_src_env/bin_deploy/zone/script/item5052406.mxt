
proc 1
	GET_ID_PLAYER_INFO 0 6 30
	if 30 = 1
		strvar num
		GET_PACK_SPACE 0 num
		if num > 5
			dec_item_num_inpak 5052406 1 10
			if 10 = 0 
				add_item_num_inpak 3550004 1 1 20

				;ADD_BIND_ITEM_NUM_INPAK 5103023 1 1 20

				add_item_num_inpak 3051004 1 1 20
			;	add_item_num_inpak 3451022 1 1 20
				add_item_num_inpak 3451004 1 1 20
				add_item_num_inpak 3751004 1 1 20
				add_item_num_inpak 3851004 1 1 20

			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留5个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		strvar num
		GET_PACK_SPACE 0 num
		if num > 5
			dec_item_num_inpak 5052406 1 10
			if 10 = 0 
				add_item_num_inpak 3550004 1 1 20
				add_item_num_inpak 3050004 1 1 20
				add_item_num_inpak 3450004 1 1 20
				add_item_num_inpak 3750004 1 1 20
				add_item_num_inpak 3850004 1 1 20

			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留5个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
endproc

