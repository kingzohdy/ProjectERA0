;����
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
		
		DEC_ITEM_NUM_INPAK 5052300 1 100
		if 100 = -1
			return
		endif
		
		add_sys_msg_start
		strvar rad
		rand rad 1000
		if rad < 46
		;	ADD_BIND_ITEM_NUM_INPAK 5012114 10 0 0
			ADD_BIND_ITEM_NUM_INPAK 5012114 10 1 20

			add_sys_msg ������10�����ڽ���

		endif
		if rad >= 46 and rad < 76
			ADD_ITEM_NUM_INPAK 5012115 1 1 20
			add_sys_msg ������1����������
		endif
		if rad >= 76 and rad < 106
			ADD_ITEM_NUM_INPAK 5012116 1 1 20
			add_sys_msg ������1�����ͱ���
		endif
		if rad >= 106 and rad < 136
			ADD_ITEM_NUM_INPAK 5012117 1 1 20
			add_sys_msg ������1�������̻�
		endif
		if rad >= 136 and rad < 236
			ADD_BIND_ITEM_NUM_INPAK 5052859 1 1 20
			add_sys_msg ������1��һС����ȯ
			if 20 = -2
				ADD_ITEM_NUM_INPAK 5012114 10 1 20
				add_sys_msg ������10�����ڽ���
			endif
		endif
		if rad >= 236 and rad < 286
			ADD_BIND_ITEM_NUM_INPAK 5052860 1 1 20
			add_sys_msg ������1��һС�޽�ȯ
			if 20 = -2
				ADD_ITEM_NUM_INPAK 5012114 10 1 20
				add_sys_msg ������10�����ڽ���
			endif
		endif
		if rad >= 286 and rad < 306
			ADD_BIND_ITEM_NUM_INPAK 5052861 1 1 20
			add_sys_msg ������1��һС�н�ȯ
			if 20 = -2
				ADD_ITEM_NUM_INPAK 5012114 10 1 20
				add_sys_msg ������10�����ڽ���
			endif
		endif
		if rad >= 306 and rad < 756
			ADD_BIND_ITEM_NUM_INPAK 5060033 1 1 20
			add_sys_msg ������1������Ӳ��
		endif
		if rad >= 756 and rad < 766
			ADD_BIND_ITEM_NUM_INPAK 5060034 1 1 20
			add_sys_msg ������1���𸣴�Կ��
		endif
		if rad >= 766 and rad < 966
			ADD_ITEM_NUM_INPAK 5012112 1 1 20
			add_sys_msg ������1������
		endif
		if rad >= 966 and rad < 971
			ADD_ITEM_NUM_INPAK 5043229 1 1 20
			add_sys_msg ������1���ƺţ��������

		endif
		if rad >= 971 and rad < 976
			ADD_ITEM_NUM_INPAK 5043230 1 1 20
			add_sys_msg ������1���ƺţ����˵�ͷ

		endif
		if rad >= 976 and rad < 981
			ADD_ITEM_NUM_INPAK 5043231 1 1 20
			add_sys_msg ������1���ƺţ��´���

		endif
		if rad >= 981 and rad < 986
			ADD_ITEM_NUM_INPAK 5043235 1 1 20
			add_sys_msg ������1���ƺţ�������˳

		endif
		if rad >= 986 and rad < 991
			ADD_ITEM_NUM_INPAK 5043236 1 1 20
			add_sys_msg ������1���ƺţ������³�

		endif
		if rad >= 991 and rad < 994
			ADD_ITEM_NUM_INPAK 5043238 1 1 20
			add_sys_msg ������1���ƺţ�����Ǭ��

		endif
		if rad >= 994 and rad < 997
			ADD_ITEM_NUM_INPAK 5043239 1 1 20
			add_sys_msg ������1���ƺţ�ǧ���Ƽ�

		endif
		if rad >= 997 and rad < 1000
			ADD_ITEM_NUM_INPAK 5043233 1 1 20
			add_sys_msg ������1���ƺţ�������

		endif
		
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
endproc



