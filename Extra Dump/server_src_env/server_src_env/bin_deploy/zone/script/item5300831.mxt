;;;;福利任务



proc 1
	STRVAR X Y M Map
	get_player_mapinfo 0 Map 2 X Y
	
	task_stat 3637 1 20
	if 20 = 2
		get_point_dist @X @Y 14000 12100 M
		if Map = 2100 or Map = 2102 or Map = 2103 or Map = 2101 or Map = 2105 or Map = 2106 or Map = 2107 or Map = 2108
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3637 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif
	endif
	task_stat 3638 1 21
	if 21 = 2
		get_point_dist @X @Y 6500 12700 M
		if Map = 2200
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3638 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif	
	endif
	task_stat 3639 1 22
	if 22 = 2
		get_point_dist @X @Y 4700 23000 M
		if Map = 2300
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3639 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif	
	
	endif
	task_stat 3640 1 23
	if 23 = 2
		get_point_dist @X @Y 21600 16600 M
		if Map = 2400
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3640 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif	
	endif
	task_stat 3641 1 24
	if 24 = 2
		get_point_dist @X @Y 22600 31300 M
		if Map = 2500
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3641 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif
	endif
	task_stat 3642 1 25
	if 25 = 2
		get_point_dist @X @Y 17400 19200 M
		if Map = 2104
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3642 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif	
	endif
	task_stat 3643 1 26
	if 26 = 2
		get_point_dist @X @Y 14200 26600 M
		if Map = 2201
			if M < 300
				effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
				effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	
				dec_item_num_inpak 5300831 1 0
				task_fini 3643 0
			else
				add_sys_msg_start
				add_sys_msg 需要在指定区域使用该物品！
				set_sys_msg_mode 3 0 0
				MSG_INFO 
			endif
		else
			add_sys_msg_start
			add_sys_msg 需要在指定地图才可使用该物品！
			set_sys_msg_mode 3 0 0
			MSG_INFO 
		endif	
	endif

endproc 
