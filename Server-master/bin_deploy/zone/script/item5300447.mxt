; 灵魂石 招图腾

proc 1
	  get_player_mapinfo 0 1 2 3 4
;	  get_point_dist @3 @4 3300 10200 10
	  if 2 = 2000 or 2 = 2001 or 2 = 2002 or 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007 
	
			dec_item_num_inpak 5300447 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0

			GET_PLAYER_MAPINFO 0 0 49 50 51
			CALL_MON 1 284017 500 30 @50 @51 40
			
			add_sys_msg_start
			add_sys_msg 一定要保护好图腾的安全！
			set_sys_msg_mode 3 0 0
			msg_info
			
			get_task_var 1067 2 32 1 12
			var 12 @40
			set_task_var 1067 2 32 @12 1
			
			;召唤图腾
			GET_TASK_VAR 1067 0 8 1 10
			VAR 10 1
			SET_TASK_VAR 1067 0 8 @10 1

	  else
			add_sys_msg_start
			add_sys_msg 请在新手村使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif
	
endproc
