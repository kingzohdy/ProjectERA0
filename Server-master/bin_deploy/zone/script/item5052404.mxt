
;;;

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 6
		DEC_ITEM_NUM_INPAK 5052404 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 6 30
			if 30 = 1
			;男
				ADD_BIND_ITEM_NUM_INPAK 3051008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3451008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3751008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3851008 1 1 0
			endif
			if 30 = 2
				ADD_BIND_ITEM_NUM_INPAK 3050008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3450008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3750008 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3850008 1 1 0

			endif
			
			ADD_BIND_ITEM_NUM_INPAK 3550008 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 3650007 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留6个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







