;��ʯ��
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 3
		add_sys_msg_start
		add_sys_msg �����ռ䲻��,��Ԥ��3�����Ͽո��ٿ�����
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad1 Rad2 Rad3
		var Rad1 0
		var Rad2 3

		while @Rad1 < @Rad2
		add Rad1 1
			rand Rad3 12		
			switch Rad3
				case 0
					ADD_ITEM_NUM_INPAK 5211202 1 0 0
				endcase
				case 1
					ADD_ITEM_NUM_INPAK 5212402 1 0 0
				endcase
				case 2
					ADD_ITEM_NUM_INPAK 5213302 1 0 0
				endcase
				case 3
					ADD_ITEM_NUM_INPAK 5213402 1 0 0
				
				endcase
				case 4
					ADD_ITEM_NUM_INPAK 5219402 1 0 0
				
				endcase
				case 5
					ADD_ITEM_NUM_INPAK 5219402 1 0 0
				
				endcase
				case 6
					ADD_ITEM_NUM_INPAK 5210002 1 0 0
				
				endcase
				case 7
					ADD_ITEM_NUM_INPAK 5210102 1 0 0
				
				endcase
				case 8
					ADD_ITEM_NUM_INPAK 5211002 1 0 0
				
				endcase
				case 9
					ADD_ITEM_NUM_INPAK 5211102 1 0 0
				
				endcase
				case 10
					ADD_ITEM_NUM_INPAK 5212002 1 0 0
				
				endcase
				case 11
					ADD_ITEM_NUM_INPAK 5212102 1 0 0
				endcase

			endswitch
		endwhile
		DEC_ITEM_NUM_INPAK 5052802 1 1
	endif

endproc
