;公会任务


proc 1

		strvar Num
		GET_PACK_SPACE 0 Num
		if Num < 6
			add_sys_msg_start
			add_sys_msg 开启此物品需要背包预留6个空格！
			set_sys_msg_mode 1 0 0
			msg_info
			return
		else
			STRVAR Ret
			dec_item_num_inpak 5030106 1 Ret
			if Ret = 0	
		
				add_item_num_inpak 3052001 1 1 0
				add_item_num_inpak 3452001 1 1 0
				add_item_num_inpak 3552001 1 1 0
				add_item_num_inpak 3652001 1 1 0
				add_item_num_inpak 3752001 1 1 0
				add_item_num_inpak 3852001 1 1 0
			endif
		endif

endproc


