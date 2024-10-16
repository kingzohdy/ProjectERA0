;5052425	夜枭时装套7天

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 4
		DEC_ITEM_NUM_INPAK 5052425 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050042 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450042 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750042 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850042 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留4个空格再开启道具！
		set_sys_msg_mode 3 0 0
	endif


endproc 