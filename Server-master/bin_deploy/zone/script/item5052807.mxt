;特典宝箱
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 1
		add_sys_msg_start
		add_sys_msg 开启宝箱会随机获得物品，请预留1个以上背包空格位置再开启！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad1 
		DEC_ITEM_NUM_INPAK 5052807 1 100
		if 100 = -1
			return
		endif
		
		rand Rad1 100
		if Rad1 < 25
			ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得40个“幻化果实”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 25 and Rad1 < 50
			ADD_ITEM_NUM_INPAK_NOLIMIT 5060014 120 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得120个“宝石碎片”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 50 and Rad1 < 68
			ADD_ITEM_NUM_INPAK_NOLIMIT 5200020 60 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得60个“星辉宝钻”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 68 and Rad1 < 73
			ADD_ITEM_NUM_INPAK_NOLIMIT 5052954 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得“100万绑银”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 73 and Rad1 < 83
			ADD_ITEM_NUM_INPAK_NOLIMIT 5250404 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得“紫晶提炼器”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 83 and Rad1 < 93
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251015 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得稀有背包“塔伯特皮包”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 93 and Rad1 < 98
			ADD_ITEM_NUM_INPAK 5600208 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得稀有坐骑“骑士之魂”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得40个“幻化果实”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if Rad1 >= 98
			ADD_ITEM_NUM_INPAK 5610305 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得稀有宠物“公爵”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 40 1 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【特典宝箱】获得40个“幻化果实”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	endif
endproc
