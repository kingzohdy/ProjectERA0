;一袋好东西
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 1
		add_sys_msg_start
		add_sys_msg 背包空间不足,请预留3个以上空格再开启！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad
		rand Rad 100
		if Rad <= 2
			ADD_ITEM_NUM_INPAK 5210001 1 0 0
			
		endif
		if Rad > 2 and Rad <= 4
			ADD_ITEM_NUM_INPAK 5210101 1 0 0
		endif
		if Rad > 4 and Rad <= 85
			ADD_ITEM_NUM_INPAK 5060101 1 0 0
		
		endif
		if Rad > 85 and Rad <= 100
			ADD_ITEM_NUM_INPAK 5060002 1 0 0

		endif
		DEC_ITEM_NUM_INPAK 5300565 1 1
	endif

endproc
