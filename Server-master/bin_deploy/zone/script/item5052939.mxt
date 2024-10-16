
;;;;金宝箱

;红色宝石包

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052939 1 10
		if 10 = 0
		GET_ID_PLAYER_INFO 0 7 30
			if 30 = 1 or 30 = 17 or 30 = 33 or 30 = 3 or 30 = 19 or 30 = 35
				rand 20 2
				if 20 = 0
					ADD_BIND_ITEM_NUM_INPAK 5210001 1 1 0
				endif
				if 20 = 1
					ADD_BIND_ITEM_NUM_INPAK 5213301 1 1 0
				endif
			endif
			
			if 30 = 2 or 30 = 18 or 30 = 34 or 30 = 4 or 30 = 20 or 30 = 36
				rand 40 2
				if 40 = 0
					ADD_BIND_ITEM_NUM_INPAK 5213301 1 1 0
				endif
				if 40 = 1
					ADD_BIND_ITEM_NUM_INPAK 5210101 1 1 0
				endif
			endif
		endif			
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
