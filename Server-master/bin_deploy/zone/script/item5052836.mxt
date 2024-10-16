;护符宝箱 

proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num > 0
		strvar Ret Rad
		dec_item_num_inpak 5052836 1 Ret
		if Ret = 0
			rand Rad 5
			switch Rad
				case 0
					add_item_num_inpak 2718110 1 1 0
				endcase
				case 1
					add_item_num_inpak 2718130 1 1 0
				endcase
				case 2
					add_item_num_inpak 2718150 1 1 0
				endcase
				case 3
					add_item_num_inpak 2718170 1 1 0
				endcase
				case 4
					add_item_num_inpak 2718190 1 1 0
				endcase
			endswitch
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法开启宝箱！
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
