;公会任务


proc 1

	get_player_mapinfo 0 1 2 3 4
;	get_point_dist @3 @4 18000 13201 10
	if 2 = 1000
;			dec_item_num_inpak 5300329 1 0
;			add_item_num_inpak 5300297 1 1 0
		task_fini 3673 1
	else
			add_sys_msg_start
			add_sys_msg 目的地不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
endproc


