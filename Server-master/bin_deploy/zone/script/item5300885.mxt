;魔法能量


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2000 or 2 = 2001 or 2 = 2002 or 2 = 2003 or 2 = 2004 or 2 = 2005 or 2 = 2006 or 2 = 2007
		dec_item_num_inpak 5300885 1 0
		task_fini 1071 0
;			get_map_npc_id 304002 10 11
;			effect_id 0 @10 playMontion(%d,"40216_5")
	else
		add_sys_msg_start
		add_sys_msg 此物品必须在天空岛使用！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

