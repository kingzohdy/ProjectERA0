



proc 1
;	strvar Rac Ret Num
;	GET_PACK_SPACE 0 Num
;	if Num = 0
;		add_sys_msg_start
;		add_sys_msg 背包空间不足！
;		set_sys_msg_mode 1 0 0
;		msg_info
;		return
;	else
		strvar Dret num1 num2 Ret1
		GET_ITEM_NUM_INPAK 5103001 1 num1
		GET_ITEM_NUM_INPAK 5102001 1 num2
		if num1 < 5
			add_sys_msg_start
			add_sys_msg 秘境精华数量不足
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		if num2 < 5
			add_sys_msg_start
			add_sys_msg 原生油数量不足
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		
		dec_item_num_inpak 5300954 1 Dret
		if Dret = 0
			dec_item_num_inpak 5103001 5 Dret
			dec_item_num_inpak 5102001 5 Dret
			ADD_ITEM_NUM_INPAK 5300955 1 1 Ret1
		endif
				
endproc 











