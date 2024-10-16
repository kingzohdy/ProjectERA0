;


proc 1
	get_player_mapinfo 0 1 2 3 4
	
	if 2 = 2300 
;			effect 0 0 playEffct(6800,6001,"effect\\entity\\9544_16.ent")
			
			GET_MON_INFO 10 11 12 13 14 15 16 17
			DESTROY_MON 100
			rand 200 10
			add 200 100
			if 200 < 108
				call_mon 1 204092 0 300 @16 @17 1
				add_sys_msg_start
				add_sys_msg 突然，他突然面目狰狞地向你扑来！
				set_sys_msg_mode 3 0 0
				msg_info

			else
				add_sys_msg_start
				add_sys_msg 药膏并没有对他造成什么影响，换个目标再使用吧！
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


