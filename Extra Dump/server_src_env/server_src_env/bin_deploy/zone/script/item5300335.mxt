;水壶空


proc 1
	  get_player_mapinfo 0 1 2 3 4

	  get_point_dist @3 @4 17964 13305 10
	  if 2 = 2200
		if 10 <= 500  
			
			dec_item_num_inpak 5300335 1 0
			ADD_ITEM_NUM_INPAK 5300336 1 1 0
			add_sys_msg_start
			add_sys_msg 找到石头后，请打开背包的任务物品栏，使用装满水的水壶！
			set_sys_msg_mode 3 0 0
			msg_info

		else
			add_sys_msg_start
			add_sys_msg 请先到达雪原取水点，再使用水壶装水！
			set_sys_msg_mode 3 0 0
			msg_info
		endif



	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


