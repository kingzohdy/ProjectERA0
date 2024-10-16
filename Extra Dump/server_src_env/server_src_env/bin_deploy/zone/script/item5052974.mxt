;新手卡之家公测特权金卡

proc 1
	get_role_var 70 8 0 20
	if 20 = 1
		strvar ret
		GET_PACK_SPACE 0 ret
		if ret > 0
			DEC_ITEM_NUM_INPAK 5052974 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052930 1 1 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留1个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
	if 20 = 0
		strvar ret
		GET_PACK_SPACE 0 ret
		if ret >= 9
			DEC_ITEM_NUM_INPAK 5052974 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3650003 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5051007 10 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5050000 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052859 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 10 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 10 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5006103 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052930 1 1 0
				var 20 1
				set_role_var 70 8 @20 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留9个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
endproc  

