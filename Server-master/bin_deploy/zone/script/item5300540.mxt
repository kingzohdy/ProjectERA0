; 1818


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2104
	
;			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
;			dec_item_num_inpak 5300333 1 0
;			add_item_num_inpak 5300549 1 1 0
		strvar ID Rad
		get_player_id 1 ID
		
;		HAS_STATUS 1249 1
		rand Rad 5
		if Rad < 3
			TARGET_ADD_STATUS 1 @ID 1249 1 180 1	
			
			get_task_var 1852 0 8 1 20
			var 20 1
			set_task_var 1852 0 8 @20 1
;			RUN_TASK_PROGRESS 0 1852 1 0	
			task_fini 1852 1
			add_sys_msg_start
			add_sys_msg 你获得了古树的祝福，快回去告诉吉森吧！
			set_sys_msg_mode 3 0 0
			msg_info	
		else
			add_sys_msg_start
			add_sys_msg 这次没能获得古树的祝福，多试几次吧！
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


