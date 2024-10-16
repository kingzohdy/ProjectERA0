;10元充值礼包


proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 2 
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		
		DEC_ITEM_NUM_INPAK 5052932 1 100
		if 100 = 0
			ADD_BIND_ITEM_NUM_INPAK 5251005 1 1 20
			ADD_BIND_ITEM_NUM_INPAK 5250101 5 1 20
			
		endif

	;	ADD_ITEM_NUM_INPAK 5012115 1 1 20
	endif
endproc



