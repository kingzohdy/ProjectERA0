;����Ӣ�ۼ���� ��ֳ� Ӣ�ۼ����

proc 1

		strvar ret
		GET_PACK_SPACE 0 ret
		if ret >= 1
			DEC_ITEM_NUM_INPAK 5060058 1 10
			if 10 = 0
				ADD_BIND_ITEM_NUM_INPAK 5060050 100 1 0
			endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣬��򿪱����鿴
			set_sys_msg_mode 4 0 0
			msg_info
		endif

endproc 
