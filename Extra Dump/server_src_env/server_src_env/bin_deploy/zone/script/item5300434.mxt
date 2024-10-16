;鸟肉


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2400
	
;			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
			dec_item_num_inpak 5300434 1 0
;			add_item_num_inpak 5300299 1 1 0
;			del_mon 1

			RUN_TASK_PROGRESS 0 1614 1 0
		
;			get_task_var 1614 0 8 1 10
;			add 10 1
;			set_task_var 1614 0 8 @10 1
;			if 10 = 1 or 10 = 2 or 10 = 3 or 10 = 4 
;				GET_MAP_NPC_ID 230020 20 1
;				mon_speech @20 2 0 1 伙计，再来点肉行吗？
;			endif
;
;			if 10 = 5 
;				
;				task_fini 1614 1
;				GET_MAP_NPC_ID 230020 20 1
;				mon_speech @20 2 0 1 终于吃饱了！
;			endif
	
			
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc
