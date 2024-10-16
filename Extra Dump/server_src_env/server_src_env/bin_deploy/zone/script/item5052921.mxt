
;;;;铁宝箱

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052921 1 10
		if 10 = 0
			strvar lv
			GET_ID_PLAYER_INFO 0 8 lv
			if lv <= 60
			
				ADD_BIND_ITEM_NUM_INPAK 5062000 1 1 0
			endif
			if lv > 60 and lv <= 80
			
				ADD_BIND_ITEM_NUM_INPAK 5062003 1 1 0
			endif
			if lv > 80 and lv <= 120
			
				ADD_BIND_ITEM_NUM_INPAK 5062009 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







