;;Ê¥µ®»ÆÀñÏä

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
		dec_item_num_inpak 5012107 1 Dret
		if Dret = 0
			rand Rad 100
			
			
			
			if Rad < 2

				ADD_ITEM_NUM_INPAK 3550013 1 1 0
				
				add_sys_msg_start
				add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®»ÆÀñÏä¡¿»ñµÃ¡°Ê¥µ®Ã±×Ó¡±Ê±×°£¡£¡
				set_sys_msg_mode 18 0 0
				svr_msg_info
			elseif Rad >= 2 and Rad < 4
				ADD_ITEM_NUM_INPAK 3050013 1 1 0
				
				add_sys_msg_start
				add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®»ÆÀñÏä¡¿»ñµÃ¡°Ê¥µ®ÉÏÒÂ¡±Ê±×°£¡£¡
				set_sys_msg_mode 18 0 0
				svr_msg_info
			elseif Rad >= 4 and Rad < 6
				ADD_ITEM_NUM_INPAK 3450013 1 1 0
				
				add_sys_msg_start
				add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®»ÆÀñÏä¡¿»ñµÃ¡°Ê¥µ®¿ã×Ó¡±Ê±×°£¡£¡
				set_sys_msg_mode 18 0 0
				svr_msg_info
			elseif Rad >= 6 and Rad < 8
				ADD_ITEM_NUM_INPAK 3750013 1 1 0
				
				add_sys_msg_start
				add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®»ÆÀñÏä¡¿»ñµÃ¡°Ê¥µ®Ğ¬×Ó¡±Ê±×°£¡£¡
				set_sys_msg_mode 18 0 0
				svr_msg_info
			elseif Rad >= 8 and Rad < 10
			
				ADD_ITEM_NUM_INPAK 3850013 1 1 0
				
				add_sys_msg_start
				add_sys_msg ¹§Ï²¡°$_player$¡±¿ªÆô¡¾Ê¥µ®»ÆÀñÏä¡¿»ñµÃ¡°Ê¥µ®ÊÖÌ×¡±Ê±×°£¡£¡
				set_sys_msg_mode 18 0 0
				svr_msg_info
			elseif Rad >= 10 and Rad < 25
				ADD_ITEM_NUM_INPAK 5012104 1 1 0
			elseif Rad >= 25 and Rad < 40
				ADD_ITEM_NUM_INPAK 5012108 1 1 0
			elseif Rad >= 40 and Rad < 41
				ADD_ITEM_NUM_INPAK 5012100 3 1 0
			elseif Rad >= 41 and Rad < 42
				ADD_ITEM_NUM_INPAK 5012101 1 1 0
			else
				ADD_ITEM_NUM_INPAK 5012105 3 1 0
			endif
			
		endif
		
	endif
	
endproc 