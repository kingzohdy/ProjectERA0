proc 1
	
	get_player_mapinfo 1 11 12 13 14
	if @11 = 2300
		get_point_dist @13 @14 3700 13000 15
		if @15 < 1000
			dec_item_num_inpak 5300603 1 0
			add_sys_msg_start
			add_sys_msg 在树根的缝隙中找到一张破旧的羊皮纸
			set_sys_msg_mode 4 0 0
			msg_info
			add_item_num_inpak 5300358 1 1 0
		else
			use_item_show_map 1 2300 5300603 3700 13000
			add_sys_msg_start
			add_sys_msg 请仔细查看藏宝图
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		use_item_show_map 1 2300 5300603 3700 13000
			add_sys_msg_start
			add_sys_msg 请仔细查看藏宝图
			set_sys_msg_mode 4 0 0
			msg_info
	endif
		

		;use_item_show_map 1 2300 5300603 3700 13000
	;if @1 = -1
		;add_sys_msg_start
		;add_sys_msg 请仔细查看地图
		;set_sys_msg_mode 4 0 0
		;msg_info
	;else
			;dec_item_num_inpak 5300603 1 0
			;add_sys_msg_start
			;add_sys_msg 在树根的缝隙中找到一张破旧的羊皮纸
			;set_sys_msg_mode 4 0 0
			;msg_info
			;add_item_num_inpak 5300358 1 1 0	
		
	;endif
	
endproc	