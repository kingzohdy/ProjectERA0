;
proc 1
	strvar Num1 Num2 Rad
;	GET_PACK_SPACE 0 Num
	
	ADD_ITEM_NUM_INPAK 5200020 2 2 Num1
	ADD_ITEM_NUM_INPAK 5200020 3 2 Num2
	rand Rad 2
	if Rad = 0
		if Num1 = -1
			add_sys_msg_start
			add_sys_msg 背包空间不足，不能开启
			set_sys_msg_mode 4 0 0
			msg_info
			return
		else
			DEC_ITEM_NUM_INPAK 5052813 1 100
			if 100 = 0
				ADD_BIND_ITEM_NUM_INPAK 5200020 2 0 0
			endif
		endif
	endif
	if Rad = 1
		if Num2 = -1
			add_sys_msg_start
			add_sys_msg 背包空间不足，不能开启
			set_sys_msg_mode 4 0 0
			msg_info
			return
		else
			DEC_ITEM_NUM_INPAK 5052813 1 100
			if 100 = 0
				ADD_BIND_ITEM_NUM_INPAK 5200020 3 0 0
			endif
		endif
	
	endif
	
endproc
