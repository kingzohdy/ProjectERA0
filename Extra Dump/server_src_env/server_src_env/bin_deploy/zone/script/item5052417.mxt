;5052417	沙鹰时装套30天

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052417 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 3050033 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3450033 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3750033 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3850033 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3550033 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留5个空格再开启道具！
		set_sys_msg_mode 3 0 0
	endif


endproc 