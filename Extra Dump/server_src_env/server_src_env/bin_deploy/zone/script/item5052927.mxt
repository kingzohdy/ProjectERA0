
;;;先锋礼包

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 3
		DEC_ITEM_NUM_INPAK 5052927 1 10
		if 10 = 0

			ADD_BIND_ITEM_NUM_INPAK 5002000 1 1 0

			ADD_BIND_ITEM_NUM_INPAK 5003000 1 1 0

			ADD_BIND_ITEM_NUM_INPAK 5251103 1 1 0
			
			ADD_BIND_ITEM_NUM_INPAK 5043127 1 1 0
			
			ADD_BIND_ITEM_NUM_INPAK 5052817 1 1 0
			
			ADD_BIND_ITEM_NUM_INPAK 5006106 3 1 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留3个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  







