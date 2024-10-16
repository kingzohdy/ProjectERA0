;木柴


proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 11267 8339 10
	if 2 = 2200

			effect 0 0 playEffct(11267,8339,"effect\\entity\\9545_16.ent")
			dec_item_num_inpak 5300327 1 0
			ADD_ITEM_NUM_INPAK 5300328 1 1 0			

	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


