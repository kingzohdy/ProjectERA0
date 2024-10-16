;木桶装水


proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 18000 13201 10
	if 2 = 2200
		if 10 <= 510  
			dec_item_num_inpak 5300329 1 0
			add_item_num_inpak 5300297 1 1 0
		
			GET_PLAYER_ID 1 54
			TARGET_ADD_STATUS 1 @54 1098 1 1 53

			add_sys_msg_start
			add_sys_msg 终于装好水了，好重……
			set_sys_msg_mode 3 0 0
		else
			add_sys_msg_start
			add_sys_msg 不是在这个区域！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
			add_sys_msg_start
			add_sys_msg 目的地不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
endproc


