; 春节·星辉豪礼

proc 1
	strvar num ret zhi gei kou
	GET_PACK_SPACE 0 num
	get_role_var 22 8 ret zhi
	
	if num < 1
		add_sys_msg_start
		add_sys_msg 背包需要1个格子才装得下哦！请先整理背包再开启
		set_sys_msg_mode 1 0 0
		msg_info
	elseif ret = 0 and zhi < 15
		DEC_ITEM_NUM_INPAK 5052296 1 kou
			
		if kou = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 gei
				
			if gei = 0
				add zhi 1
				set_role_var 22 8 @zhi ret
			endif
		endif
	elseif ret = 0 and zhi > 14
		DEC_ITEM_NUM_INPAK 5052296 1 kou
			
		if kou = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012114 10 1 0
		endif
	else
	endif
endproc
