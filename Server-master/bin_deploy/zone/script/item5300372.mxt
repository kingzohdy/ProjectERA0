;蝎子果酱

proc 1
	
	
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 4300 3100 10
	
	if 2 = 2300
		if 10 <= 500
		;	dec_item_num_inpak 5300372 1 0
			
			get_task_var 1525 0 8 1 10
			var 10 1
			set_task_var 1525 0 8 @10 1

	
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
		else
			add_sys_msg_start
			add_sys_msg 目标不在指定地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	endif
 
endproc
