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
		strvar Rad1 Ret
		
		DEC_ITEM_NUM_INPAK 5052806 1 Ret
		if Ret = -1
			return
		endif
		
		rand Rad1 100
		if Rad1 < 25
			ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���15�����û���ʵ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 25 and Rad1 < 50
			ADD_ITEM_NUM_INPAK_NOLIMIT 5060014 40 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���40������ʯ��Ƭ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 50 and Rad1 < 68
			ADD_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���20�����ǻԱ��ꡱ��
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 68 and Rad1 < 84
			ADD_ITEM_NUM_INPAK_NOLIMIT 5030122 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿��á�ˮ��������Ȩ��[10��]����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 84 and Rad1 < 89
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251030 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿��á�ʱװ��λ��չ����
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 89 and Rad1 < 94
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251401 3 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���3��������������λ�ơ���
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 94 and Rad1 < 99
			ADD_ITEM_NUM_INPAK 5610411 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���ϡ�г��С��������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���15�����û���ʵ����
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if Rad1 >= 99
			ADD_ITEM_NUM_INPAK 5600210 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���ϡ������������衱��
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
				add_sys_msg_start
				add_sys_msg ��ϲ��ҡ�$_player$�����������䱦�䡿���15�����û���ʵ����
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	endif
endproc
