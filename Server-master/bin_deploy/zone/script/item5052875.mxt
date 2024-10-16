;普通议会配方匣


proc 1
	strvar num
	GET_PACK_SPACE 0 num
	if num > 0
		strvar rad ret
		dec_item_num_inpak 5052875 1 ret
		if ret = 0
			rand rad 5
			if rad = 0
				ADD_BIND_ITEM_NUM_INPAK 5254269 1 1 0
				return
			endif
			if rad = 1
				ADD_BIND_ITEM_NUM_INPAK 5254270 1 1 0
				return
			endif
			if rad = 2
				ADD_BIND_ITEM_NUM_INPAK 5254271 1 1 0
				return
			endif
			if rad = 3
				ADD_BIND_ITEM_NUM_INPAK 5256014 1 1 0
				return
			endif
			if rad = 4
				ADD_BIND_ITEM_NUM_INPAK 5256015 1 1 0
				return
			;	add_item_num_inpak 5254269 1 1 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启！
		set_sys_msg_mode 4 0 0
		msg_info	
	endif
endproc 
proc 2
	strvar num
	GET_PACK_SPACE 0 num
	if num = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启！
		set_sys_msg_mode 4 0 0
		msg_info	
		SET_SCRIPT_INT_RET -1
	endif
endproc 