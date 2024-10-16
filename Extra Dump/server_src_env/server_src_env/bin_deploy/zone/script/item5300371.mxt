;蒲草笼
proc 1

			get_player_mapinfo 0 1 2 3 4

			  if 2 = 2300

					dec_item_num_inpak 5300371 1 0
					ADD_ITEM_NUM_INPAK 5300351 1 1 0
					
;					GET_MON_INFO 0 30 31 32 33 34 35 36
;					CALL_MON 1 226010 100 60 @35 @36 0
					DESTROY_MON 1
			  else
					add_sys_msg_start
					add_sys_msg 目标不在本地图中！
					set_sys_msg_mode 4 0 0
					msg_info
			  endif
	
endproc
			