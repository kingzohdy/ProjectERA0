;��¶
;��¶+�Ͷ�˶��=��ģ��Ʒ��У��Ҽ�ʹ����Ʒ������2����Ʒ�õ�1����Ʒ��

proc 1
	GET_PACK_SPACE 0 20
	if 20 < 1
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣡
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_ITEM_NUM_INPAK 5052306 0 10
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052305 1 11
		DEC_ITEM_NUM_INPAK 5052306 1 12
		if 11 = 0 and 12 = 0
			ADD_BIND_ITEM_NUM_INPAK 5052307 1 1 13
			if 13 = 0
				add_sys_msg_start
				add_sys_msg ʹ�ø�¶���Ͷ�˶�����һ����ģ��Ʒ���
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		call 100
	endif
endproc

proc 100
	add_sys_msg_start
	add_sys_msg ���ı�����ȱ�١��Ͷ�˶����������Ʒ���ڡ������̵ꡱ����
	set_sys_msg_mode 4 0 0
	msg_info
endproc 