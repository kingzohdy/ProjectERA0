;�������� 

proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num > 0
		strvar Ret Rad
		dec_item_num_inpak 5052840 1 Ret
		if Ret = 0
			rand Rad 5
			switch Rad
				case 0
					add_item_num_inpak 2718311 1 1 0
				endcase
				case 1
					add_item_num_inpak 2718331 1 1 0
				endcase
				case 2
					add_item_num_inpak 2718351 1 1 0
				endcase
				case 3
					add_item_num_inpak 2718371 1 1 0
				endcase
				case 4
					add_item_num_inpak 2718391 1 1 0
				endcase
			endswitch
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷��������䣡
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
