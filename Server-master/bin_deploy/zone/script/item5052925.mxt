
;;;;钻石头宝箱

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052925 1 10
		if 10 = 0
			rand 20 100
			if 20 < 30
				ADD_BIND_ITEM_NUM_INPAK 5234000 1 1 0
			endif
			if 20 >= 30 and 20 < 60
				ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
			endif
			if 20 >= 60 and 20 < 90
				ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
			endif
			if 20 >= 90 and 20 < 100
				ADD_BIND_ITEM_NUM_INPAK 5052859 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







