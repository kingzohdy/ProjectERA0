;轰鸣炸药
proc 1
	  strvar x y
	  get_player_mapinfo 0 1 2 x y

		get_point_dist @x @y 11800 9900 10
		  if 1 = 10300 or 1 = 10700 or 1 = 11000 or 1 = 11300
			if 10 <= 500  
				
				NEW_LIFE_MON 290003 1 @x @y 0 0 0 0 3 0 0
;				ADD_ITEM_NUM_INPAK 5300336 1 1 0
				dec_item_num_inpak 5300514 1 0
				get_task_var 3144 0 8 1 50
				add 50 1
				set_task_var 3144 0 8 @50 1
				if 50 = 3
					task_fini 3144 1	
				endif
				
			else
				add_sys_msg_start
				add_sys_msg 请在采集场使用轰鸣炸药！
				set_sys_msg_mode 3 0 0
				msg_info

			endif
		  else
			add_sys_msg_start
			add_sys_msg 请在指定地图使用炸药！
			set_sys_msg_mode 3 0 0
			msg_info
		  
		  
		  endif



endproc 

