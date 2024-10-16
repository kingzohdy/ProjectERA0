;纯净的核心
;任务——1064
;作用——使用后删除，并完成任务
proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2000 OR 2 = 2001 OR 2 = 2002 OR 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007 
			
;			dec_item_num_inpak 5300520 1 0
			task_fini 1064 0
			
			get_map_npc_id 351000 10 11
			effect_id 0 @10 playMontion(%d,"400400_4")


	ELSE
			add_sys_msg_start
			add_sys_msg 目标不再指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	ENDIF
endproc

