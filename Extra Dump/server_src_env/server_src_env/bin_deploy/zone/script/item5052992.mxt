;�߼���ʯ�� 5052992
proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 1
		DEC_ITEM_NUM_INPAK 5052992 1 10
		if 10 = 0
			rand 40 100
			if 40 < 10
				;���˹�����ʯ
				GET_ID_PLAYER_INFO 0 7 30
				;�ж�ְҵ
				if 30 = 1 or 30 = 3 or 30 = 17 or 30 = 19 or 30 = 33 or 30 = 35
					;���﹥
					ADD_BIND_ITEM_NUM_INPAK 5210006 1 1 0
				else	
					;��ħ��
					ADD_BIND_ITEM_NUM_INPAK 5210106 1 1 0
				endif
			endif
			
			if 40 >= 10 and 40 < 22
				ADD_BIND_ITEM_NUM_INPAK 5211006 1 1 0
			endif
			
			if 40 >= 22 and 40 < 34
				ADD_BIND_ITEM_NUM_INPAK 5211106 1 1 0
			endif
			
			if 40 >= 34 and 40 < 46
				ADD_BIND_ITEM_NUM_INPAK 5211206 1 1 0
			endif
			
			if 40 >= 46 and 40 < 56
				ADD_BIND_ITEM_NUM_INPAK 5212006 1 1 0
			endif
			
			if 40 >= 56 and 40 < 68
				ADD_BIND_ITEM_NUM_INPAK 5212406 1 1 0
			endif
			
			if 40 >= 68 and 40 < 79
				ADD_BIND_ITEM_NUM_INPAK 5213306 1 1 0
			endif
			
			if 40 >= 79 and 40 < 89
				ADD_BIND_ITEM_NUM_INPAK 5213406 1 1 0
			endif
			
			if 40 >= 89
				ADD_BIND_ITEM_NUM_INPAK 5213206 1 1 0
			endif
		endif	
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��1���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  