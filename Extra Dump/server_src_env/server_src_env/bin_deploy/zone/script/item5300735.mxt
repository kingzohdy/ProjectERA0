;;;;

proc 1

			
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 16207 3025 10

	if 2 = 2200
		if 10 <= 2000
			effect 0 0 playEffct(16207,3025,"effect\\entity\\720000_6.ent")
			dec_item_num_inpak 5300735 1 0
	;		del_mon 1
			task_fini 1405 1
		else
			add_sys_msg_start
			add_sys_msg 需要在指定区域才可使用！
			set_sys_msg_mode 3 0 0
			msg_info
		endif

	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
		

endproc  