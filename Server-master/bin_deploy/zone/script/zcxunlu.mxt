
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b哈喽，圣诞节可是少不了装扮一翻哟！
	add_talk #b#b每年这个时候，圣诞老公公都为大家准备了很多很多圣诞装扮！
	add_talk #b#b如果你也要装扮一翻，不要忘了每天向我领取礼箱咯！嘿嘿！
	add_talk #b#b今年也要来场华丽丽的变装派对！噢耶！
	
		option 100 #【圣诞变装派对】
		
	talk
endproc

proc 100
	dialog 1
	get_player_daily_flag 20 20 21
	get_player_daily_flag 30 21 31
	get_player_daily_flag 40 22 41
	STRVAR time
	GET_DAYONLINE 0 time
	
	add_talk #b#b#b#b#b#b#b#c6d00d2【圣诞变装派对】#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b “#c186506圣诞黄礼箱#n”
	add_talk  每天累积在线#c1865061、2、4小时#n可分别领取1次礼箱
	add_talk #b 打开礼箱：将有机会获得“#c005ebe圣诞时装#n”！！
	
	if 40 = 0 and 41 = 1
		add_talk #b#b
		add_talk #b#c005ebe今天已领取了3次礼箱，明天记得再来领取哟！#n
	endif
	
	
	if 20 = 0 and 21 = 0
		option 101 #领取“圣诞黄礼箱”（累积在线1小时）
	endif
	if 30 = 0 and 31 = 0
		if time > 3600
			option 102 #领取“圣诞黄礼箱”（累积在线2小时）
		endif
	endif
	if 40 = 0 and 41 = 0
		if time > 7200
			option 103 #领取“圣诞黄礼箱”（累积在线4小时）
		endif
	endif
		
	talk
endproc

proc 101
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 3600
		get_player_daily_flag ret 20 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 3 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 驯鹿宝宝神秘兮兮的递给你3个“圣诞黄礼箱”
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 20 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出1格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线1小时的“圣诞黄礼箱”大礼包咯
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 3600
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 驯鹿宝宝仍在整理礼箱中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 102
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 7200
		get_player_daily_flag ret 21 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 4 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 驯鹿宝宝神秘兮兮的递给你4个“圣诞黄礼箱”
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 21 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出1格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线2小时的“圣诞黄礼箱”大礼包咯
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 7200
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 驯鹿宝宝仍在整理礼箱中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 103
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 14400
		get_player_daily_flag ret 22 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 5 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 驯鹿宝宝神秘兮兮的递给你5个“圣诞黄礼箱”
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 22 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出1格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线4小时的“圣诞黄礼箱”大礼包咯
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 14400
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 驯鹿宝宝仍在整理礼箱中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
