;巴掌

proc 1

	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108
	  	get_task_var 1318 4 8 1 14
		if 14 = 1
			;get_player_id 0 20
			;effect_id 0 @20 playMontion(%d,"9566_16")
			dec_item_num_inpak 5300416 1 0
			get_task_var 1318 1 8 1 11
			var 11 1
			set_task_var 1318 1 8 @11 1
			
		else
			add_sys_msg_start
			add_sys_msg 好像没效果，先听听他们说完再使用试试！
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

