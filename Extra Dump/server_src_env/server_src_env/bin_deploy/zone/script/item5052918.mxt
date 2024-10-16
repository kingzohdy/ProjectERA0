;零纪元YY频道礼包

proc 1
	
	strvar ret num
	GET_PACK_SPACE 0 num
	
	if num >= 5
		DEC_ITEM_NUM_INPAK 5052918 1 ret
		if ret = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5043265 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250101 2 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5050000 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc 