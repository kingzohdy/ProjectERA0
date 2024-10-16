;牧草


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2400
	
;			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
			dec_item_num_inpak 5300436 1 0
;			add_item_num_inpak 5300299 1 1 0
;			del_mon 1
			get_task_var 1626 0 8 1 10
			add 10 1
			set_task_var 1626 0 8 @10 1
			if 10 = 1 or 10 = 2 or 10 = 3 or 10 = 4 
				add_sys_msg_start
				add_sys_msg 马儿需要更多牧草才能被驯服
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			if 10 = 5 
				
				GET_MON_INFO 10 11 12 13 14 15 16 17 
				NEW_OWNER_TMP_ITEM 1460005 @16 @17 0 0 60 0 0 0 0 0
			;	APPLY_TMP_ITEM 1460005 60 1
				DESTROY_MON 1
				add_sys_msg_start
				add_sys_msg 在牧草的攻势下，马儿已对你产生了好感，快骑回去交差吧。
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
