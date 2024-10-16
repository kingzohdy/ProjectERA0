;经典宝箱
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num < 1
		add_sys_msg_start
		add_sys_msg 开启宝箱会获得物品，请预留1个以上背包空格位置再开启！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar Rad1 Ret
		
		DEC_ITEM_NUM_INPAK 5052806 1 Ret
		if Ret = -1
			return
		endif
		
		rand Rad1 100
		if Rad1 < 25
			ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得15个“幻化果实”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 25 and Rad1 < 50
			ADD_ITEM_NUM_INPAK_NOLIMIT 5060014 40 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得40个“宝石碎片”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 50 and Rad1 < 68
			ADD_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得20个“星辉宝钻”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 68 and Rad1 < 84
			ADD_ITEM_NUM_INPAK_NOLIMIT 5030122 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得“水晶徽章特权卡[10天]”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 84 and Rad1 < 89
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251030 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得“时装栏位扩展”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 89 and Rad1 < 94
			ADD_ITEM_NUM_INPAK_NOLIMIT 5251401 3 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得3个“精制坐骑栏位牌”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if Rad1 >= 94 and Rad1 < 99
			ADD_ITEM_NUM_INPAK 5610411 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得稀有宠物“小蓝龙”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得15个“幻化果实”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if Rad1 >= 99
			ADD_ITEM_NUM_INPAK 5600210 1 0 20
			if 20 = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得稀有坐骑“幽灵蝠鲨”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
			if 20 = -2
				ADD_ITEM_NUM_INPAK_NOLIMIT 5233000 15 1 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”开启【经典宝箱】获得15个“幻化果实”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	endif
endproc
