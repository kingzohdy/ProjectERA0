;染丝工具包

proc 1
	strvar ret ret_1
	GET_PACK_SPACE 1 ret
	
	if ret >= 4
		DEC_ITEM_NUM_INPAK 5300161 1 ret_1
		if ret_1 = 0
			ADD_ITEM_NUM_INPAK 5301021 1 1 0
			ADD_ITEM_NUM_INPAK 5300162 1 1 0
			ADD_ITEM_NUM_INPAK 5300163 1 1 0
			ADD_ITEM_NUM_INPAK 5300164 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
	
endproc 