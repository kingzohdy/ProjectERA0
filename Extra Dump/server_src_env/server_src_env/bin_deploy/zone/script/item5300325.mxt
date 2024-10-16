;挖掘工具


proc 1
		
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 7000 6100 10
	if 2 = 2200
		if 10 <= 500  
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
			dec_item_num_inpak 5300325 1 0
			add_item_num_inpak 5300295 1 1 0

			add_sys_msg_start
			add_sys_msg 挖到了一个青铜箱子！
			set_sys_msg_mode 3 0 0
			msg_info
			task_fini 1425 0
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


