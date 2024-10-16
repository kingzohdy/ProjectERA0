;装满水的水壶


proc 1
	  get_player_mapinfo 0 1 2 3 4

;	  get_point_dist @3 @4 17964 13305 10
	  if 2 = 2200
			GET_MON_INFO 10 11 12 13 14 15 16 17 
			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9547_16.ent")	
			
;			dec_item_num_inpak 5300336 1 0
;			ADD_ITEM_NUM_INPAK 5300339 1 1 0
			del_mon 1

	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


