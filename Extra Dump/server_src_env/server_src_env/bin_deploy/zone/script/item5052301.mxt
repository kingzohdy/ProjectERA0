;�𸣴�
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 1 
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣡
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		GET_ITEM_NUM_INPAK 5060034 1 60
		if 60 = 0
			add_sys_msg_start
			add_sys_msg ��û�н𸣴�Կ�ף����ܴ򿪽𸣴���
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		
		DEC_ITEM_NUM_INPAK 5052301 1 100
		DEC_ITEM_NUM_INPAK 5060034 1 101
		if 100 = -1 and 101 = -1
			return
		endif

		add_sys_msg_start
		strvar rad
		rand rad 10000
		if rad < 3
			ADD_ITEM_NUM_INPAK 5611417 1 1 20

			if 20 = -2
				ADD_BIND_ITEM_NUM_INPAK 5060033 1 1 20
					
				add_sys_msg ������1������Ӳ��
			else
				add_sys_msg_start
				add_sys_msg ���˵�$_player$�ڽ𸣴��￪���ˡ��������ޡ��������Ǻ��˵�ͷ��
				set_sys_msg_mode 6 0 0
				svr_msg_info
			endif
		endif
		if rad >= 3 and rad < 3153
			ADD_BIND_ITEM_NUM_INPAK 5060033 1 1 20
			add_sys_msg ������1������Ӳ��
		endif
		if rad >= 3153 and rad < 3403
			ADD_ITEM_NUM_INPAK 5012113 3 1 20
			add_sys_msg ������3�����˽�ħ���̻�

		endif
		if rad >= 3403 and rad < 3603
			ADD_ITEM_NUM_INPAK 3550014 1 1 20
			add_sys_msg ������1������ñ�ӣ�7�죩

		endif
		if rad >= 3603 and rad < 3803
			ADD_ITEM_NUM_INPAK 3850014 1 1 20
			add_sys_msg ������1���������ף�7�죩

		endif
		if rad >= 3803 and rad < 4003
			ADD_ITEM_NUM_INPAK 3050014 1 1 20
			add_sys_msg ������1���������£�7�죩

		endif
		if rad >= 4003 and rad < 4203
			ADD_ITEM_NUM_INPAK 3450014 1 1 20
			add_sys_msg ������1�����ڿ��ӣ�7�죩

		endif
		if rad >= 4203 and rad < 4403
			ADD_ITEM_NUM_INPAK 3750014 1 1 20
			add_sys_msg ������1������Ь�ӣ�7�죩

		endif
		
		
		if rad >= 4403 and rad < 5803
			ADD_ITEM_NUM_INPAK 5219600 2 1 20
			add_sys_msg ������2��������ʵ

		endif
		if rad >= 5803 and rad < 6803
			ADD_ITEM_NUM_INPAK 5233000 2 1 20
			add_sys_msg ������2���û���ʵ

		endif
		if rad >= 6803 and rad < 9800
			ADD_ITEM_NUM_INPAK 5050000 2 1 20
			add_sys_msg ������2��С����

		endif
		if rad >= 9800 and rad < 9900
			ADD_ITEM_NUM_INPAK 5043237 1 1 20
			add_sys_msg ������1���ƺţ��︣�Ǹ��ա�

		endif
		if rad >= 9900
			ADD_ITEM_NUM_INPAK 5043232 1 1 20
			add_sys_msg ������1���ƺţ��������\(^o^)/

		endif
		
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	
endproc





