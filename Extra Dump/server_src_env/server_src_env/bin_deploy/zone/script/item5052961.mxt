;三百元礼包

proc 1
    strvar ret pmid zhi ret2

	GET_PLAYER_ID 1 pmid
	GET_ROLE_VAR 65 8 ret zhi
	
	if zhi = 0
		add_money @pmid 500 61 ret
		if ret = 0
			add zhi 1
			set_role_var 65 8 @zhi 0
		endif
	elseif zhi = 1
		add_money @pmid 1000 61 ret
		if ret = 0
			add zhi 1
			set_role_var 65 8 @zhi 0
		endif
	elseif zhi = 2
		DEC_ITEM_NUM_INPAK 5052961 1 ret2
		if ret2 = 0
			add_money @pmid 1500 61 ret
			if ret = 0
				add zhi 1
				set_role_var 65 8 @zhi 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已经开启了3次【三百元礼包】，无法再开启更多次
		set_sys_msg_mode 3 0 0
		MSG_INFO
	endif
endproc
