
;乐天春节活动
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我正在收集春节硬币，如果你能给我一些，我愿意用我的宝贝和你交换。
	add_talk #b#b#c186506春节商人只在1月27日--2月10日在时间城逗留

		option 150 #(买卖)打开商店
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 200 #【马年送豪礼  马上有星辉】
	endif
	
	talk
endproc 

proc 150
	OPEN_NPC_SHOP 
endproc

proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#b#b 【马年送豪礼，马上有星辉】
	add_talk #b#b#b#b#b#b#b#c1865061月27日-2月10日#n
	add_talk #b#b
	add_talk #b 每天累积在线2小时可领取1次“星辉宝钻”
	add_talk #b#b#b#c005ebe每次可领取多达20个“星辉宝钻”！
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 210 #恭喜发财，星辉拿来
	endif
	
	talk 
endproc

proc 210
	strvar num ol chunjie zhi
	GET_PACK_SPACE 0 num
	GET_DAYONLINE 0 ol
	GET_EVENT_STAT chunjie 302
	GET_PLAYER_DAILY_FLAG 0 5 zhi
	
	if chunjie = 1
		if zhi = 0
			if ol > 7200
				if @num < 1
					ADD_SYS_MSG_START
					ADD_SYS_MSG 请空出1格背包格子再前来领取
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				else
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 0
					
					set_player_daily_flag 0 5 1
					
					add_sys_msg_start
					add_sys_msg 莎莎霸气的塞给了你20个“星辉宝钻”！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				var 10 7200
				sub 10 @ol
				dev 10 60
				add 10 1
				
				ADD_SYS_MSG_START
				ADD_SYS_MSG 莎莎仍在筹备“马上有星辉”豪礼中，请$@10$分钟后再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你今天已领取过累积在线2小时的“马上有星辉”豪礼，不能太贪心哦
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		add_sys_msg_start
		add_sys_msg 马年“春节豪礼”活动已经结束了，暂时无法领取
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
