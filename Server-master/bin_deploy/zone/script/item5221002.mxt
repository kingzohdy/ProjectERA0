;3级绿宝石碎片(物防)


proc 1

	GET_ITEM_NUM_INPAK 5221002 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg 材料不足，请凑齐5个。
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 5
		ADD_ITEM_NUM_INPAK 5211002 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5221002 5 1
		Else
		add_sys_msg_start
		add_sys_msg 背包已满，请预留1个空位。
		set_sys_msg_mode 4 0 0
		msg_info
		ENDIF
	ENDIF
endproc		