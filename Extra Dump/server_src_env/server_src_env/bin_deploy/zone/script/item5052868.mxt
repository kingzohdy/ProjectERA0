
;新手礼包

proc 1
	strvar ret pmid lv ret1 ret2 ret3 
	
	add_item_num_inpak 5050510 3 2 ret1
	add_item_num_inpak 5251001 1 2 ret2
	add_item_num_inpak 5052869 1 2 ret3
	
	
	if ret1 = 0 and ret2 = 0 and ret3 = 0
		DEC_ITEM_NUM_INPAK 5052868 1 ret
		if ret = 0

			
				ADD_BIND_ITEM_NUM_INPAK 5050510 3 1 0
				ADD_BIND_ITEM_NUM_INPAK 5251001 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5052869 1 1 0
		
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 