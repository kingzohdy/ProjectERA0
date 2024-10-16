;天灯
;任务——
proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist  @3 @4 14088 12049 10
	
	if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108
		if 10 <= 500
			effect 0 0 playEffct(14088,12049,"effect\\entity\\9516_16.ent")
			dec_item_num_inpak 5300401 1 0

			task_fini 1336 0

		
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		else
			add_sys_msg_start
			add_sys_msg 目标不再指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
 
endproc
