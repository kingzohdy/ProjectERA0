;�����ļ⵶

proc 1
		GET_ITEM_NUM_INPAK 5300380 0 10
		;����
		GET_ITEM_NUM_INPAK 5300381 0 11
		;�ֲ�
		GET_ITEM_NUM_INPAK 5300382 0 12
		;�Դ�
	
		
		if 10 < 1 and 11 < 1 and 12 < 1
			add_sys_msg_start
			add_sys_msg ���Ȳɼ�һЩ���ʵ����
			set_sys_msg_mode 3 0 0
			msg_info

		endif
		if 10 >= 1
			dec_item_num_inpak 5300380 1 0
			rand 50 4
			add 50 100
			if 50 = 100
			;�ֲ�
				ADD_ITEM_NUM_INPAK 5300381 1 1 0
				
				add_sys_msg_start
				add_sys_msg ������Ƭ�����ϸ������ǩ����ңԶ����Ҫ�ٶ������£�
				set_sys_msg_mode 3 0 0
				msg_info
				return
			endif
			if 50 = 101
			;�Դ�
				ADD_ITEM_NUM_INPAK 5300382 1 1 0
				add_sys_msg_start
				add_sys_msg �����ϸ���˵���ǩ�Ѿ��ܽ��ˣ��ٶ������°ɣ�
				set_sys_msg_mode 3 0 0
				msg_info
				return
			endif
			if 50 = 102
			;����
				ADD_ITEM_NUM_INPAK 5300383 1 1 0
				dec_item_num_inpak 5300379 1 0
				return
			endif
			if 50 = 103
			;��ϸ
				ADD_ITEM_NUM_INPAK 5300384 1 1 0
				add_sys_msg_start
				add_sys_msg ���õ�����������ǩ�����Ĺ�ϸ����ȥ��Щ���ʵ���ڰɣ�
				set_sys_msg_mode 3 0 0
				msg_info
				return
				
			endif
		endif
		if 11 >= 1
			dec_item_num_inpak 5300381 1 0
			rand 51 3
			add 51 100
			switch 51
				case 100
					ADD_ITEM_NUM_INPAK 5300382 1 1 0
					add_sys_msg_start
					add_sys_msg �����ϸ���˵���ǩ�Ѿ��ܽ��ˣ��ٶ������°ɣ�
					set_sys_msg_mode 3 0 0
					msg_info
					return
				endcase
				case 101
					ADD_ITEM_NUM_INPAK 5300383 1 1 0
					dec_item_num_inpak 5300379 1 0
					return
				endcase
				case 102
					ADD_ITEM_NUM_INPAK 5300384 1 1 0
					add_sys_msg_start
					add_sys_msg ���õ�����������ǩ�����Ĺ�ϸ����ȥ��Щ���ʵ���ڰɣ�
					set_sys_msg_mode 3 0 0
					msg_info
					return
				endcase
			
			endswitch
		
		endif
		if 12 >= 1
		
			dec_item_num_inpak 5300382 1 0
			rand 52 2
			add 52 100
			switch 52 
				case 100
					ADD_ITEM_NUM_INPAK 5300383 1 1 0
					dec_item_num_inpak 5300379 1 0
					return
				endcase 
				case 101
					ADD_ITEM_NUM_INPAK 5300384 1 1 0
					add_sys_msg_start
					add_sys_msg ���õ�����������ǩ�����Ĺ�ϸ����ȥ��Щ���ʵ���ڰɣ�
					set_sys_msg_mode 3 0 0
					msg_info
				endcase
			
			endswitch
		endif


endproc 
















