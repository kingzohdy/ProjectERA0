;小鸟


proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 6800 6001 10
	if 2 = 2200
		if 10 <= 500  
			GET_PLAYER_MAPINFO 0 20 21 22 23
			effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9544_16.ent")
			
			dec_item_num_inpak 5300326 1 0
			add_sys_msg_start
			add_sys_msg 任务完成
			set_sys_msg_mode 3 0 0
			msg_info
			task_fini 1428 0
		else
			add_sys_msg_start
			add_sys_msg 不是在这个区域！
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


