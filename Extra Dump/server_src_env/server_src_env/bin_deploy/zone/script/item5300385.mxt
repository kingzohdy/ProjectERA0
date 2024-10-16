;麻醉药剂

proc 1

	  get_player_mapinfo 0 1 2 3 4
	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108

			dec_item_num_inpak 5300385 1 0
			ADD_ITEM_NUM_INPAK 5300414 1 1 0
			
;倒下的祭祀			
			;GET_MON_INFO 0 0 0 0 0 0 20 21
			;call_mon 1 293086 0 60 @20 @21 0
			
			
			GET_PLAYER_ID 1 30
			TARGET_ADD_STATUS 1 @30 1219 1 600 1	
			DESTROY_MON 0
			


	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif


endproc 

