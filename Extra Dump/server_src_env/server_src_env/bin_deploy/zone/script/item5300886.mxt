;充盈的蓝晶核心


proc 1

	  get_player_mapinfo 0 1 2 3 4
	  get_point_dist @3 @4 6000 13300 10

	  if 1 = 10200
		if 10 <= 500  
			dec_item_num_inpak 5300886 1 0
			add_item_num_inpak 5300887 1 1 0
			task_fini 1084 0
	
;			get_map_npc_id 304002 10 11
;			effect_id 0 @10 playMontion(%d,"40216_5")
		else
			add_sys_msg_start
			add_sys_msg 不是在这个区域！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
	  else
			add_sys_msg_start
			add_sys_msg 此物品必须在永夜镇（简单难度副本）才可使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc 

