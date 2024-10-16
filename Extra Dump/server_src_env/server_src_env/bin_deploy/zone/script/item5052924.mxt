
;;;;钻扣宝箱

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052924 1 10
		if 10 = 0
			rand 20 100
			if 20 < 60
				ADD_BIND_ITEM_NUM_INPAK 5213538 1 1 0
			endif
			if 20 >= 60 and 20 < 80
				ADD_BIND_ITEM_NUM_INPAK 5213539 1 1 0
			endif
			if 20 >= 80 and 20 < 100
				ADD_BIND_ITEM_NUM_INPAK 5060068 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







