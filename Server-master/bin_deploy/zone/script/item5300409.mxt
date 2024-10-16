;天灯
;任务——
proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist  @3 @4 30167	45807 10
	
	if 2 = 1000
		if 10 <= 300
			effect 0 0 playEffct(30167,45807,"effect\\entity\\9516_16.ent")
			dec_item_num_inpak 5300409 1 0

			task_fini 4636 0

		
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
