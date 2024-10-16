;情人节时装礼盒
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 5
		add_sys_msg_start
		add_sys_msg 背包空间不足,请预留5个空格！
		set_sys_msg_mode 4 0 0
		msg_info
		return
		
	else
		
		DEC_ITEM_NUM_INPAK 5052929 1 100
		if 100 = -1
			return
		endif
		ADD_ITEM_NUM_INPAK 3550016 1 1 20
		
		ADD_ITEM_NUM_INPAK 3850016 1 1 20
		
		ADD_ITEM_NUM_INPAK 3050016 1 1 20
		
		ADD_ITEM_NUM_INPAK 3450016 1 1 20
		
		ADD_ITEM_NUM_INPAK 3750016 1 1 20

	endif
endproc
