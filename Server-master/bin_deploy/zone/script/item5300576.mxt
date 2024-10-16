;救赎符文
proc 1

	  strvar x y
	  get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 7900 9000 10

		  if 1 = 3100 
				if 10 <= 500
					GET_PLAYER_MAPINFO 0 20 21 22 23
					effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9900000_15.ent")	
					dec_item_num_inpak 5300576 1 0
					task_fini 1866 1
				else
					add_sys_msg_start
					add_sys_msg 不是在指定区域哦！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
;			NEW_LIFE_MON 290003 1 @x @y 0 0 0 0 3 0 0
;			ADD_ITEM_NUM_INPAK 5300336 1 1 0
			
		  else
			add_sys_msg_start
			add_sys_msg 请在指定地图使用！
			set_sys_msg_mode 3 0 0
			msg_info
		  
		  endif
		  
endproc 
