;明亮的火把 选中使用

proc 1
	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 3300
			GET_MON_INFO 10 11 12 13 14 15 16 17
			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9567_16.ent")
;			dec_item_num_inpak 5300356 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0	
	
			del_mon 1
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif

endproc
