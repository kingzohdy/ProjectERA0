
;;;;含笑花
proc 1

	GET_ITEM_NUM_INPAK 5300721 1 11
	if 11 < 8
		add_sys_msg_start
		add_sys_msg 含笑花不足，请凑齐8个。
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 8
		ADD_ITEM_NUM_INPAK 5300722 1 1 12
		IF 12 = 0
			DEC_ITEM_NUM_INPAK 5300721 8 1
		Else
			add_sys_msg_start
			add_sys_msg 背包已满，请预留1个空位。
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	ENDIF
endproc