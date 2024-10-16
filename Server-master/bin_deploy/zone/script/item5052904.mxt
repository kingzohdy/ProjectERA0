
;;;;官网神马包

proc 1
	get_role_var 7 8 0 20
	if 20 = 1
		strvar ret
		GET_PACK_SPACE 0 ret
		if ret > 0
			DEC_ITEM_NUM_INPAK 5052904 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK 5054000 1 1 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留1个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
	if 20 = 0
		strvar ret
		GET_PACK_SPACE 0 ret
		if ret >= 8
			DEC_ITEM_NUM_INPAK 5052904 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK 3650003 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5052898 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5051007 5 1 0
				ADD_BIND_ITEM_NUM_INPAK 5052859 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5050000 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5250101 3 1 0
				ADD_BIND_ITEM_NUM_INPAK 5054006 1 1 0
				ADD_BIND_ITEM_NUM_INPAK 5054000 1 1 0
				var 20 1
				set_role_var 7 8 @20 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留8个空格再开启道具！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif

endproc  







