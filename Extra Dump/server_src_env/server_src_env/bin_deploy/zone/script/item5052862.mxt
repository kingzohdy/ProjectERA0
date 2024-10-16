
;新手礼包1~10

proc 1
	strvar ret pmid lv ret1 ret2 ret3 
	GET_ID_PLAYER_INFO 0 8 lv	
;	
		add_item_num_inpak 5000000 5 2 ret1
		add_item_num_inpak 5001000 5 2 ret2
		add_item_num_inpak 5052864 1 2 ret3
		if ret1 = 0 and ret2 = 0 and ret3 = 0

			DEC_ITEM_NUM_INPAK 5052862 1 ret
			if ret = 0
				ADD_BIND_ITEM_NUM_INPAK 5000000 5 1 0
				ADD_BIND_ITEM_NUM_INPAK 5001000 5 1 0
				
				ADD_BIND_ITEM_NUM_INPAK 5052864 1 1 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		endif

endproc 