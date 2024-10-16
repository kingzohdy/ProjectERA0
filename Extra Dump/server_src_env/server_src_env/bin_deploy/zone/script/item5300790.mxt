;火把


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3304
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
;			dec_item_num_inpak 5300285 1 0			
;			add_item_num_inpak 5300339 1 1 0
		strvar Mid Mhp Mhp1
		GET_TARGET_INFO	3 1 Mid
;		GET_MON_HP_PERCENT @Mid 30
		
		GET_TARGET_INFO 3 2 Mhp
		var Mhp1 @Mhp
		sub Mhp1 1
		if @Mhp1 < @Mhp
			TARGET_ADD_STATUS 3 @Mid 3026 1 60 1	
			RUN_TASK_PROGRESS 0 2054 1 1
		else
			add_sys_msg_start
			add_sys_msg 目标血量过多！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
;;;;;;;;;;;持续掉血
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


