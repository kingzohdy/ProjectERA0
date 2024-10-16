;公测尊享大礼包

proc 1

	strvar ret zhi
	GET_PACK_SPACE 0 ret
	GET_ROLE_VAR 75 8 0 zhi
	if zhi < 1
		if ret >= 10
			DEC_ITEM_NUM_INPAK 5052913 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052912 5 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5610217 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060023 20 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052419 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052892 2 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251005 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052949 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5041000 10 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 30 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 30 1 0
				
				add zhi 1
				SET_ROLE_VAR 75 8 @zhi 0
				
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留10个空格再开启道具！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已开启过公测尊享大礼包，无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







