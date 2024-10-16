;终极vip独宠包
proc 1
	strvar Num Dret
	
	GET_PACK_SPACE 0 Num
	if Num < 5
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 1 0 0
		msg_info
		return
		
	else
		dec_item_num_inpak 5052248 1 Dret
		
		if Dret = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030143 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030144 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030145 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030146 1 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5610217 1 1 0
		
		endif
	endif
	
endproc