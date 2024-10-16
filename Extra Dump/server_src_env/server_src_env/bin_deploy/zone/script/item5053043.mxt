

proc 1
	GET_PACK_SPACE 0 10
	if 10 >= 3


		strvar ret
		dec_item_num_inpak 5053043 1 ret
		if ret = 0
;		ADD_MONEY 0 100 21 0
		
			ADD_BIND_ITEM_NUM_INPAK 5250400 5 1 0
			rand 50 100
			if 50 < 10
				ADD_BIND_ITEM_NUM_INPAK 5250401 1 1 0
			endif
			rand 51 100
			if 51 < 50
				ADD_BIND_ITEM_NUM_INPAK 5250402 1 1 0
			endif
		endif
	else
	
		add_sys_msg_start
		add_sys_msg 工具箱可能会随机开出不同数量道具，背包请预留3个空格再开启
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 
