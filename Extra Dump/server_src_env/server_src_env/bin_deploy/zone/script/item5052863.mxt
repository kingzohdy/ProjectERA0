
;新手礼包

proc 1
	strvar ret pmid lv ret1 ret2 ret3 ret4 ret5
	
	add_item_num_inpak 5043127 1 2 ret1
	add_item_num_inpak 5610412 1 2 ret2
	add_item_num_inpak 5052892 1 2 ret3
	add_item_num_inpak 5200030 5 2 ret4
	add_item_num_inpak 5200040 5 2 ret5
	if ret1 = 0 and ret2 = 0 and ret3 = 0 and ret4 = 0 and ret5 = 0
	
		DEC_ITEM_NUM_INPAK 5052863 1 ret
		if ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5043127 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 5610412 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 5052892 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 5200030 5 1 0
			ADD_BIND_ITEM_NUM_INPAK 5200040 5 1 0
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启！
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
endproc 