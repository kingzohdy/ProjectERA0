;;Ê¥µ®ÂÌÀñÏä

proc 1
	strvar Rac Ret Num
	GET_PACK_SPACE 0 Num
	
	if Num < 1
		add_sys_msg_start
		add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
		set_sys_msg_mode 1 0 0
		msg_info
		return
		
	else
		strvar Dret Rad Rad2
		dec_item_num_inpak 5012106 1 Dret
		if Dret = 0
			var 20 0
			while 20 < 1
			add 20 1
				rand Rad 1000
				if Rad < 4
					strvar Ret1
					ADD_ITEM_NUM_INPAK 5610413 1 1 Ret1
					
					if Ret1 = 0
						add_sys_msg_start
						add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®ÂÌÀñÏä¡¿»ñµÃ¡°Ñ±Â¹±¦±¦¡±³èÎï£¡£¡
						set_sys_msg_mode 18 0 0
						svr_msg_info
					endif
					if Ret1 = -2
					;	ADD_ITEM_NUM_INPAK 5012104 1 1 0
						var 20 0
					endif
				elseif Rad >= 4 and Rad < 250
					ADD_ITEM_NUM_INPAK 5012104 1 1 0
				elseif Rad >= 250 and Rad < 350
					ADD_ITEM_NUM_INPAK 5012108 1 1 0
				elseif Rad >= 350 and Rad < 375
					ADD_ITEM_NUM_INPAK 5012100 3 1 0
				elseif Rad >= 375 and Rad < 400
					ADD_ITEM_NUM_INPAK 5012101 1 1 0
				else
					ADD_ITEM_NUM_INPAK 5012105 3 1 0
				endif
			endwhile
		endif
	endif
endproc 