

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0

		DEC_ITEM_NUM_INPAK 5052936 1 10
		if 10 = 0
			GET_PLAYER_BASE_ATT 1 1 30 1
			if 30 < 50
				ADD_BIND_ITEM_NUM_INPAK 5250300 1 1 0
			endif
			if 30 >= 50 and 30 < 60
				ADD_BIND_ITEM_NUM_INPAK 5250301 1 1 0
			endif
			if 30 >= 60 and 30 < 70
				ADD_BIND_ITEM_NUM_INPAK 5250302 1 1 0
			endif
			if 30 >= 70 and 30 < 80
				ADD_BIND_ITEM_NUM_INPAK 5250303 1 1 0
			endif
			if 30 >= 80 
				ADD_BIND_ITEM_NUM_INPAK 5250304 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 