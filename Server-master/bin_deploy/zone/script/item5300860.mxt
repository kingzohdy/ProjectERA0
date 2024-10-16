;促生激素


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3400
		strvar Mid Deid
		GET_TARGET_INFO 3 1 Mid
		GET_ID_MON_INFO @Mid 1 Deid 0 0 0 0 0
		if Deid = 206369 or Deid = 233012
			strvar x y hp
			GET_TARGET_INFO 3 3 x
			GET_TARGET_INFO 3 4 y
			GET_TARGET_INFO 3 2 hp
			if hp > 0
				effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\5300860.ent")
				dec_item_num_inpak 5300860 1 0
				add_item_num_inpak 5300861 1 1 0
				DESTROY_MON 1
			else
				add_sys_msg_start
				add_sys_msg 怪物已死亡，无法获得物品！
				set_sys_msg_mode 3 0 0
				msg_info
			
			endif
		else
			add_sys_msg_start
			add_sys_msg 该物品只能对毒蝎子使用
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
