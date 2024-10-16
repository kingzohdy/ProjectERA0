;工作报告


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3302
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
;			dec_item_num_inpak 5300285 1 0			
;			add_item_num_inpak 5300339 1 1 0
;			task_fini 1547 1
		strvar Mid pMid
		get_player_id 1 pMid
		PLAYER_TRIGGER_MON_PRESS 1 @pMid 1000 213030 0
		
		GET_MAP_NPC_ID 304066 Mid 1
		mon_speech @Mid 2 0 1 小贼，你敢骂我！
		task_fini 1568 1
	  else
			add_sys_msg_start
			add_sys_msg 此物品需要在阿努比斯祭坛使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif
endproc


