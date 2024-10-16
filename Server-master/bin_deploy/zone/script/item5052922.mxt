
;;;;镶银宝箱

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052922 1 10
		if 10 = 0
			rand 20 100
			if 20 < 25
				ADD_BIND_ITEM_NUM_INPAK 5219600 1 1 0
			endif
			if 20 >= 25 and 20 < 50
				ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
			endif
			if 20 >= 50 and 20 < 75
				ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
			endif
			if 20 >= 75 and 20 < 80
				ADD_BIND_ITEM_NUM_INPAK 5060023 1 1 0
			endif
			if 20 >= 80 and 20 < 100
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







