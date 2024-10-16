;宝石镶嵌符碎片(绑定)


proc 1

	GET_ITEM_NUM_INPAK 5209013 1 11
	if 11 < 5
		add_sys_msg_start
		add_sys_msg 材料不足，请凑齐5个。
		set_sys_msg_mode 4 0 0
		map_msg_info
	ENDIF
	if 11 >= 5
		ADD_ITEM_NUM_INPAK 5209011 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5209013 5 1
		Else
		add_sys_msg_start
		add_sys_msg 背包已满，请预留一个空位。
		set_sys_msg_mode 4 0 0
		map_msg_info
		ENDIF
	ENDIF
endproc		