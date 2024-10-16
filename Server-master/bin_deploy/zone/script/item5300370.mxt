;水晶瓶

proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 5200 5000 10
	
	if 2 = 2300
		if 10 <= 510
			dec_item_num_inpak 5300370 1 0
			ADD_ITEM_NUM_INPAK 5300350 1 1 0
		
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		else
			add_sys_msg_start
			add_sys_msg 目标不在指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
 
endproc
