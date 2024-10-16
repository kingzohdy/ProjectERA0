;血液采集器


proc 1
	  get_player_mapinfo 0 1 2 3 4

	  get_point_dist @3 @4 8022 21669 10
	  if 2 = 2200
			
			ADD_ITEM_NUM_INPAK 5300289 1 1 0


	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


