;5052421	星际时装套永久

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052421 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050038 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450038 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750038 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850038 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3550038 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留5个空格再开启道具！
		set_sys_msg_mode 3 0 0
	endif


endproc 