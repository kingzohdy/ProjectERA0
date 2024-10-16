;2号药膏


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2104
	
			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
			dec_item_num_inpak 5300562 1 0
;			add_item_num_inpak 5300549 1 1 0

			NEW_LIFE_MON 293149 1 @16 @17 0 0 0 0 50 0 0 
			del_mon 1
			
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


