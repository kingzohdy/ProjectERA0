proc 1
	
	get_player_mapinfo 1 11 12 13 14
	if @11 = 2300
		get_point_dist @13 @14 3500 3100 15
		if @15 < 1000
			dec_item_num_inpak 5300604 1 0
			add_sys_msg_start
			add_sys_msg 在一个像獠牙的巨石旁边，你找到了豪斯镇长的手提箱
			set_sys_msg_mode 4 0 0
			msg_info
			add_item_num_inpak 5300359 1 1 0
		else
			use_item_show_map 1 2300 5300604 3500 3100
			add_sys_msg_start
			add_sys_msg 请仔细查看藏宝图
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		use_item_show_map 1 2300 5300604 3500 3100
			add_sys_msg_start
			add_sys_msg 请仔细查看藏宝图
			set_sys_msg_mode 4 0 0
			msg_info
	endif

	;use_item_show_map 1 2300 5300604 3500 3100

	;if @1 = -1
		;add_sys_msg_start
		;add_sys_msg 请仔细查看地图
		;set_sys_msg_mode 4 0 0
		;msg_info
	;else
			;dec_item_num_inpak 5300604 1 0
			;add_sys_msg_start
			;add_sys_msg 在一个像獠牙的巨石旁边，你找到了豪斯镇长的手提箱
			;set_sys_msg_mode 4 0 0
			;msg_info
			;add_item_num_inpak 5300359 1 1 0	
		
	;endif
	
endproc	