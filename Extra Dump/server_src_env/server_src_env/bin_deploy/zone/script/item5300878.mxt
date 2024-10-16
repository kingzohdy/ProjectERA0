

proc 1

	strvar ret
	dec_item_num_inpak 5300878 1 ret
	if ret = 0
		strvar Num
		GET_PACK_SPACE 1 Num
		if Num = 0
			add_sys_msg_start
			add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
			set_sys_msg_mode 1 0 0
			msg_info
			return
		
		else
		
			add_item_num_inpak 5300879 1 1 0
		
		endif
	endif
endproc 