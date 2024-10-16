
proc 1
	strvar num
	GET_PACK_SPACE 0 num
	if num > 0
		dec_item_num_inpak 5052159 1 10
		if 10 = 0 
		;	add_item_num_inpak 5210000 1 2 Ret
			ADD_BIND_ITEM_NUM_INPAK 5052161 6 1 20
			
		endif
	else
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


