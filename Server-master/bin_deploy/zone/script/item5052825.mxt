;�������� 

proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num > 0
		strvar Ret Rad
		dec_item_num_inpak 5052825 1 Ret
		if Ret = 0
			rand Rad 5
			switch Rad
				case 0
					add_item_num_inpak 2718206 1 1 0
				endcase
				case 1
					add_item_num_inpak 2718226 1 1 0
				endcase
				case 2
					add_item_num_inpak 2718246 1 1 0
				endcase
				case 3
					add_item_num_inpak 2718266 1 1 0
				endcase
				case 4
					add_item_num_inpak 2718286 1 1 0
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
