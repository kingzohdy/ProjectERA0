
;;;先锋礼包

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 5
		DEC_ITEM_NUM_INPAK 5052400 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 6 30
			if 30 = 1
			;男
				ADD_BIND_ITEM_NUM_INPAK 3051001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3451001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3751001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3851001 1 1 0
			endif
			if 30 = 2
				ADD_BIND_ITEM_NUM_INPAK 3050001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3450001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3750001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 3850001 1 1 0

			endif
			
			ADD_BIND_ITEM_NUM_INPAK 3550001 1 1 0
		;	ADD_BIND_ITEM_NUM_INPAK 3651001 1 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留5个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







