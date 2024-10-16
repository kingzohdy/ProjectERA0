;¸½Ä§±¦Ïä 

proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num > 0
		strvar Ret Rad
		dec_item_num_inpak 5241422 1 Ret
		if Ret = 0
			rand Rad 4
			switch Rad
				case 0
					add_item_num_inpak 5240222 1 1 0
				endcase
				case 1
					add_item_num_inpak 5240422 1 1 0
				endcase
				case 2
					add_item_num_inpak 5240622 1 1 0
				endcase
				case 3
					add_item_num_inpak 5240822 1 1 0
				endcase
			endswitch
		endif
	else
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¬ÎÞ·¨¿ªÆô±¦Ïä£¡
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
