;夺宝奇兵 


proc 2
	
	rand 50 100

	if @50 < 40
	
	rand 30 100000
	if @30 < 1
		ADD_ITEM_NUM_INPAK 5600208 1 1 123
		switch 123
			case 0
				add_sys_msg_start
				add_sys_msg 史无前例的好运气!$_player$开启宝箱获得一个木马坐骑，祝贺他
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endcase
			case -1
				add_sys_msg_start
				add_sys_msg 你的包裹已满
				set_sys_msg_mode 3 0 0
				msg_info
			endcase
			case -2
				call 2
			endcase
		endswitch
	endif
	if @30 >= 1 and @30 < 2000
		ADD_ITEM_NUM_INPAK 5233000 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一颗幻化果实
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 2000 and @30 < 7000
		ADD_ITEM_NUM_INPAK 5219600 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个遗忘果实
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 7000 and @30 < 15000
		ADD_ITEM_NUM_INPAK 5233500 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个基因果实
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 15000 and @30 < 22000
		ADD_ITEM_NUM_INPAK 5006100 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一瓶变身药水
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 22000 and @30 < 29000
		ADD_ITEM_NUM_INPAK 5006101 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一瓶变身药水
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 29000 and @30 < 36000
		ADD_ITEM_NUM_INPAK 5006102 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一瓶变身药水
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

	if @30 >= 36000 and @30 < 42000
		ADD_ITEM_NUM_INPAK 5006103 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一瓶变身药水
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	if @30 >= 42000 and @30 < 43000
		ADD_ITEM_NUM_INPAK 5063100 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一把智者秘钥
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	if @30 >= 43000 and @30 < 57000
		ADD_ITEM_NUM_INPAK 5052815 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一小罐绑银
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 57000 and @30 < 65000
		ADD_ITEM_NUM_INPAK 5230001 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个宠物经验果
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 65000 and @30 < 68000
		ADD_ITEM_NUM_INPAK 5230002 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个宠物经验果
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 68000 and @30 < 70000
		ADD_ITEM_NUM_INPAK 5230003 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个宠物经验果
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 70000 and @30 < 71000
		ADD_ITEM_NUM_INPAK 5230004 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个宠物经验果
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 71000 and @30 < 72000
		ADD_ITEM_NUM_INPAK 5230005 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个宠物经验果
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 72000 and @30 < 75000
		ADD_ITEM_NUM_INPAK 5250101 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一颗慧然果子
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 75000 and @30 < 77000
		ADD_ITEM_NUM_INPAK 5250102 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一颗独悟果子
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 77000 and @30 < 78000
		ADD_ITEM_NUM_INPAK 5251001 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个背包扣带
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 78000 and @30 <= 80000
		ADD_ITEM_NUM_INPAK 5062000 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一张藏宝图
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if @30 >= 80000
		ADD_ITEM_NUM_INPAK 5052111 1 1 123
		if 123 = 0
			add_sys_msg_start
			add_sys_msg 你开启宝箱获得一个神奇锦囊
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你的包裹已满
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	

	endif

	if @50 >= 40 and @50 < 99
			ID_PLAYER_BASE_ATT 0 1 1 41 1
			mul 41 @41
		rand 20 100
		if @20 < 30
			
			ADD_EXP 0 @41 123   ;单人加经验
			if 123 < 0
				add_sys_msg_start
				add_sys_msg 您当前存储的经验值已经达到上限，请提升等级，否则无法继续获得经验
				set_sys_msg_mode 3 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg 你获到了$@41$点经验
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
		endif
		if @20 > 29
			dev 41 10
			STRVAR song
			GET_EVENT_STAT song 300
			STRVAR liuyi
			GET_EVENT_STAT liuyi 88
			STRVAR wuyi
			GET_EVENT_STAT wuyi 84
			if song = 1
				MUL 41 3
			elseif liuyi = 1
				MUL 41 2
			elseif wuyi = 1
				MUL 41 5
			else
			endif
			ADD_MONEY 0 @41 20 123 ;单人加金钱
			if 123 < 0
				add_sys_msg_start
				add_sys_msg 您包裹中的银币已经达到上限
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			;获得银币系统有提示
		endif
	endif

	if @50 >= 99
				get_player_id 0 60
				RAND 40 3
				SWITCH 40
				CASE 0 
					TARGET_ADD_STATUS 1 @60 1009 1 5 123
					add_sys_msg_start
					add_sys_msg 宝箱中散出奇怪的气体迷晕了你
					set_sys_msg_mode 3 0 0
					msg_info

				ENDCASE 
				case 1
					TARGET_ADD_STATUS 1 @60 1057 1 5 123
					add_sys_msg_start
					add_sys_msg 宝箱中的精灵把你变成了一头小猪
					set_sys_msg_mode 3 0 0
					msg_info

				endcase
				case 2
					TARGET_ADD_STATUS 1 @60 1057 2 5 123
					add_sys_msg_start
					add_sys_msg 宝箱中的精灵把你变成了一个布娃娃
					set_sys_msg_mode 3 0 0
					msg_info
				endcase
				ENDSWITCH
	endif



endproc

