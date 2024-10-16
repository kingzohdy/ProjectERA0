;500元充值礼包


proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 2
		add_sys_msg_start
		add_sys_msg 请预留2个背包空格再开启礼包！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		
		DEC_ITEM_NUM_INPAK 5052935 1 100
		if 100 = 0
			ADD_ITEM_NUM_INPAK 5052203 1 1 20
			ADD_BIND_ITEM_NUM_INPAK 5250102 10 1 20
	
			
		endif
	endif
	;	ADD_ITEM_NUM_INPAK 5012115 1 1 20

endproc



