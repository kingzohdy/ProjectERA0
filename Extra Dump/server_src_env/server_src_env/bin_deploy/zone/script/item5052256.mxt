

;5052256	һ��ո�µ��ؼ�
;12����ɫ�ؼ�

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 1
		DEC_ITEM_NUM_INPAK 5052256 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 7 30
			if 30 = 1
			;սʿ
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0
			endif
			if 30 = 2
			;��ʦ
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0
			endif
			if 30 = 3
			;ǹ��
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif
			if 30 = 4
			;����
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
			
			if 30 = 17
			;�ػ�
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0		
			endif			
			if 30 = 33
			;����
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0		
			endif			
			if 30 = 18
			;����
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0		
			endif			
			if 30 = 34
			;����
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0		
			endif			
			if 30 = 19
			;��ɱ
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif	
            if 30 = 35
			;����
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif	
			if 30 = 20
			;ʥ��
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
			if 30 = 36
			;Ԧ��
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
	    endif

	
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬��Ԥ��1���ո��ٿ������ߣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  