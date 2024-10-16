;100元充值礼包


proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 3
		add_sys_msg_start
		add_sys_msg 请预留3个背包空格再开启礼包！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		
		DEC_ITEM_NUM_INPAK 5052934 1 100
		if 100 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 20
			ADD_BIND_ITEM_NUM_INPAK 5060023 20 1 20
			ADD_BIND_ITEM_NUM_INPAK 3650001 1 1 20
			
			
		endif
	endif
	;	ADD_ITEM_NUM_INPAK 5012115 1 1 20

endproc



