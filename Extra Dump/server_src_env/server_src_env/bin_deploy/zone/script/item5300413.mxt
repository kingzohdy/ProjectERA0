;满水的水桶
proc 1

	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108
	  
			dec_item_num_inpak 5300413 1 0
;			ADD_ITEM_NUM_INPAK 5300413 1 1 0
			DESTROY_MON 1
			task_fini 1317 0
;			new_mon 293088 1 5100 4900 500 0 0 0 0

;清醒的祭祀
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif


endproc 

