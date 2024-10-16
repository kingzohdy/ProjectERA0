; 暗影药膏 目标蓝色灯笼草

proc 1
	  get_player_mapinfo 0 1 2 3 4
	  if 2 = 2000 or 2 = 2001 or 2 = 2002 or 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007 
	  
		GET_TARGET_INFO 3 1 40
		GET_MON_HP_PERCENT @40 30
		if @30 < 99
			dec_item_num_inpak 5300449 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0
			strvar X Y
			GET_TARGET_INFO 3 3 X
			GET_TARGET_INFO 3 4 Y
			effect 0 0 playEffct($@X$,$@Y$,"effect\\entity\\250001_19.ent")
			task_fini 1073 0
			DESTROY_MON 0

		else
			add_sys_msg_start
			add_sys_msg 蓝色灯笼草血量过多，请先削减目标血量再使用药膏
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif


	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif
	
endproc
