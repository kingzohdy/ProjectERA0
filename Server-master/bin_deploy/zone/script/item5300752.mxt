;干扰器



proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3404
	
;			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
;			dec_item_num_inpak 5300434 1 0
;			add_item_num_inpak 5300299 1 1 0
		task_fini 2015 1
		
	  else
			add_sys_msg_start
			add_sys_msg 此物品必须在鸣沙陷地使用！
			set_sys_msg_mode 1 0 0
			msg_info
	  endif

endproc
