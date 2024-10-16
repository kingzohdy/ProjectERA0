;金熔炉凭证

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 1
		DEC_ITEM_NUM_INPAK 5052340 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 8 30
			if 30 < 40
			ADD_BIND_ITEM_NUM_INPAK 5052310 1 1 0
			endif
			if 30 >= 40 and 30 < 50
			ADD_BIND_ITEM_NUM_INPAK 5052316 1 1 0
			endif
			if 30 >= 50 and 30 < 60
			ADD_BIND_ITEM_NUM_INPAK 5052317 1 1 0
			endif			
			if 30 >= 60 and 30 < 70
			ADD_BIND_ITEM_NUM_INPAK 5052318 1 1 0
			endif
			if 30 >= 70 and 30 < 80
			ADD_BIND_ITEM_NUM_INPAK 5052319 1 1 0
			endif
			if 30 >= 80 and 30 < 90
			ADD_BIND_ITEM_NUM_INPAK 5052328 1 1 0
			endif
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc  
