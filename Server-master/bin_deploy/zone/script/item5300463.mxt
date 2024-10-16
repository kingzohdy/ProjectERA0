;号角
proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist  @3 @4 31500 45800 10
	
	if 2 = 1000
		if 10 <= 500
		
		strvar X Y
		GET_PLAYER_MAPINFO 0 0 0 X Y
		effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
		effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
		
		dec_item_num_inpak 5300463 1 0
		task_fini 1102 0
		
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		else
			add_sys_msg_start
			add_sys_msg 不在指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
 
endproc