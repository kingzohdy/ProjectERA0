;战斗力保险符碎片


proc 1

	GET_ITEM_NUM_INPAK 5209012 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg 材料不足，请凑齐5个。
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	if 11 >= 5
		ADD_ITEM_NUM_INPAK 5209010 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5209012 5 1
		Else
		add_sys_msg_start
		add_sys_msg 背包已满，请预留一个空位。
		set_sys_msg_mode 4 0 0
		map_msg_info
		ENDIF
	ENDIF
endproc		