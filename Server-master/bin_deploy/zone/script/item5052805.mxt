;���䱦��
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 1
		add_sys_msg_start
		add_sys_msg �������������Ʒ����Ԥ��1�����ϱ����ո�λ���ٿ�����
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad1 
		
		DEC_ITEM_NUM_INPAK 5052805 1 100
		if 100 = -1
			return
		endif
		
		rand Rad1 100
		if Rad1 < 22
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 1 0
		endif
		if Rad1 >= 22 and Rad1 < 44
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 1 0
		endif
		if Rad1 >= 44 and Rad1 < 66	
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 2 1 0
		endif
		if Rad1 >= 66 and Rad1 < 96 
			ADD_ITEM_NUM_INPAK_NOLIMIT 5030121 1 1 0
		endif	
		if Rad1 >= 96 and Rad1 < 98
			ADD_ITEM_NUM_INPAK 5060015 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿��á��ҡ��ּ���ң�
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 1 0
			endif
		endif
		if Rad1 >= 98 and Rad1 < 100
			ADD_ITEM_NUM_INPAK 5060016 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿��á������ּ���ң�
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 1 0
			endif
		endif
		
	endif

endproc
