; ���ڡ���ʺ���

proc 1
	strvar num ret zhi rad gei kou job
	GET_PACK_SPACE 0 num
	get_role_var 64 8 ret zhi
	
	if num < 1
		add_sys_msg_start
		add_sys_msg ������Ҫ1�����Ӳ�װ����Ŷ�������������ٿ���
		set_sys_msg_mode 1 0 0
		msg_info
	elseif ret = 0 and zhi < 1
		DEC_ITEM_NUM_INPAK 5052299 1 kou
			
		if kou = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213204 1 1 gei
				
			if gei = 0
				add zhi 1
				set_role_var 64 8 @zhi ret
			endif
		endif
	elseif ret = 0 and zhi > 0
		DEC_ITEM_NUM_INPAK 5052299 1 kou
			
		if kou = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012114 10 1 0
		endif
	else
	endif
endproc
