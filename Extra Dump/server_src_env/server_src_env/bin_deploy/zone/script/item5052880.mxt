;普通议会配方匣


proc 1
	strvar num
	GET_PACK_SPACE 0 num
	if num > 0
		strvar rad ret
		dec_item_num_inpak 5052880 1 ret
		if ret = 0
			rand rad 8
			if rad = 0
				ADD_BIND_ITEM_NUM_INPAK 5256284 1 1 0
				return
			endif
			if rad = 1
				ADD_BIND_ITEM_NUM_INPAK 5256285 1 1 0
				return
			endif
			if rad = 2
				ADD_BIND_ITEM_NUM_INPAK 5256286 1 1 0
				return
			endif
			if rad = 3
				ADD_BIND_ITEM_NUM_INPAK 5256287 1 1 0
				return
			endif
			if rad = 4
				ADD_BIND_ITEM_NUM_INPAK 5256288 1 1 0
				return
			;	add_item_num_inpak 5254269 1 1 0
			endif
			if rad = 5
				ADD_BIND_ITEM_NUM_INPAK 5256289 1 1 0
				return
			endif
			if rad = 6
				ADD_BIND_ITEM_NUM_INPAK 5256290 1 1 0
				return
			endif
			if rad = 7
				ADD_BIND_ITEM_NUM_INPAK 5256291 1 1 0
				return
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启！
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
proc 2
	strvar num
	GET_PACK_SPACE 0 num
	if num = 0

		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 