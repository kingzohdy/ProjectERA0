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
			dec_item_num_inpak 5030105 1 Ret
			if Ret = 0
				add_item_num_inpak 3052000 1 1 0
				add_item_num_inpak 3452000 1 1 0
				add_item_num_inpak 3552000 1 1 0
				add_item_num_inpak 3652000 1 1 0
				add_item_num_inpak 3752000 1 1 0
				add_item_num_inpak 3852000 1 1 0
			endif
		endif

endproc


