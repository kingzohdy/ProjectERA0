;������Ƭ

proc 1

		strvar ret num
		GET_ITEM_NUM_INPAK 5060103 1 num
		if num >= 30
			
			GET_PACK_SPACE 0 ret
			if ret >= 1
				DEC_ITEM_NUM_INPAK 5060103 30 10
				if 10 = 0
					ADD_BIND_ITEM_NUM_INPAK 5060005 1 1 0
				endif
			else
				add_sys_msg_start
				add_sys_msg �����ռ䲻�㣡
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ϳɺ���Ͻ���Ҫ30��������Ƭ��
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
endproc 
