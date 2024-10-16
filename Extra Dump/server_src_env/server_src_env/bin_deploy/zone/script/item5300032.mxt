;封印侍卫的瓶子
proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 15517 7816 10
	if 2 = 2100
		if 10 <= 500  
		CALL_MON 1 231022 100 600 15517 7816 1
		dec_item_num_inpak 5300032 1 0  
		add_sys_msg_start
		add_sys_msg 快快杀掉掠食者之魂，将其永久封印！
		set_sys_msg_mode 4 0 0
		msg_info
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif
	
endproc

		