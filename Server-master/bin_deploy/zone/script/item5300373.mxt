;ɯ��ֽ
proc 1
	get_item_num_inpak 5300373 0 10 
	get_item_num_inpak 5300351 0 11 
	get_item_num_inpak 5300352 0 12
	
		if 10 != 0
			
			if 11 != 0
				
				if 12 != 0
					
					dec_item_num_inpak 5300373 1 0
					dec_item_num_inpak 5300351 1 0
					dec_item_num_inpak 5300352 6 0
					
					add_item_num_inpak 5300372 1 1 0
					
				
				else
					
					add_sys_msg_start
					add_sys_msg ȱ�ٴ��̵�Ұ������
					set_sys_msg_mode 4 0 0
					msg_info
				
				endif
			
			else
				
				add_sys_msg_start
				add_sys_msg �ϳ���ҪɳĮ��Ы����
				set_sys_msg_mode 4 0 0
				
				msg_info
			
			endif
		
		else
		    add_sys_msg_start
			add_sys_msg �ϳ���Ҫɯ��ֽ����
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif

endproc
			