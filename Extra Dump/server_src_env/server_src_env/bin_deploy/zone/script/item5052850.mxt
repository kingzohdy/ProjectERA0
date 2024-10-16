
;

proc 1
	strvar ret num

	GET_PACK_SPACE 0 num
	if num >= 2
		DEC_ITEM_NUM_INPAK 5052850 1 ret
		if ret = 0
			ADD_BIND_ITEM_NUM_INPAK 3653003 1 1 0
			ADD_BIND_ITEM_NUM_INPAK 5052862 1 1 0
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 