;梦莎
 
proc 1	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不知什么时候，青春如过隙白驹；不知什么时候，梦想已悄然遗忘；不知什么时候，童心也烟消云散。我很庆幸，在青春即将逝去的当口，还能和【时间城幼儿园】的小可爱一起参与童年的游戏，回忆起童年的记忆，填补这份纯真!
	add_talk #b#b
	add_talk #b#b#cff00806月7日晚上24点整，梦莎就会带着孩子们回到幼儿园。
	add_talk #b#b
	add_talk #b#b#c186506欢乐奖励（嘉年华获得的银币大幅度提高）
	
		TASK_ADD_OPTION 4446 0 100
		TASK_ADD_OPTION 4447 0 101
		TASK_ADD_OPTION 4448 0 102
	
	task_stat 4447 1 20
	GET_ITEM_NUM_INPAK 5241520 1 10
	;任务提交
		if @20 = 3 and @10 < 1 
			option 160 很抱歉，我不小心把哨子弄丢了！
		;elseif @20 = 2
		;	task_fini 4447 1
		;else
		endif
		
	task_stat 4448 1 30
	GET_ITEM_NUM_INPAK 5241521 1 12
		if @30 = 3 and @12 < 1 
			option 162 很抱歉，我不小心把哨子弄丢了！
		;elseif @30 = 2
		;	task_fini 4448 1
		;else
		endif
	
	GET_PLAYER_DAILY_FLAG 0 9 21
	;0表示未设置
	if 21 = 0
	option 151 领取在线奖励
	endif
	
	talk
	
endproc

proc 100
	TASK_DO_OPTION 4446 0
endproc
proc 101
	TASK_DO_OPTION 4447 0
endproc

proc 102
	TASK_DO_OPTION 4448 0
endproc



;领取在线奖励子面板
proc 150
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
    
	GET_ID_PLAYER_INFO 0 19 12
	DEV 12 10000
	MUL 12 50

	dialog 1
	add_talk $_npc$:
	add_talk #b#b每天早上6点重置在线时间，当天在线时间可累计。
	add_talk #b#b累计在线2小时即#cff4e00可领取$@12$点经验奖励#n
	add_talk #b#b
	add_talk #b#b您今日在线时间为：$@10$时$@11$分

    option 151 领取奖励

	option 1 返回
	talk
endproc


;领取奖励
proc 151
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 9 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，您今日在线时间不足2小时，不能领取奖励。
		option 1 返回
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 9 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 13
			DEV 13 10000
			MUL 13 50
			
			ADD_EXP 0 @13 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 160
	dialog 1
	add_talk $_npc$:
	add_talk #b#b别太自责，我的朋友，我这里还有一些新的哨子，不过这次我不能将它免费送给你，人们总是对轻易得到的东西不太珍惜，所以，你需要付出点银币为代价。
	add_talk #b#b特别提醒：由于是儿童节期间，嘉年华现在开出的银币可是翻了5倍哟。
	add_talk #b#b
	option 161 #我愿用2000银币换一周陪伴提利昂的机会！
	
	talk
endproc

;扣2000银币，给予1个蓝色哨子
proc 161 
	strvar ret
	get_id_player_info 0 14 ret
	if @ret >= 2000
		call 201
	else
		call 204
		add_sys_msg_start
		add_sys_msg 银币不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 2000 20 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY 0 2000 20 12
			if 12 = 0
				ADD_BIND_ITEM_NUM_INPAK 5241520 1 1 13
				add_sys_msg_start
				add_sys_msg 获得哨子
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 162
    dialog 1
	add_talk $_npc$:
	add_talk #b#b别太自责，我的朋友，我这里还有一些新的哨子，不过这次我不能将它免费送给你，人们总是对轻易得到的东西不太珍惜，所以，你需要付出点银币为代价。
	add_talk #b#b 特别提醒：由于是儿童节期间，嘉年华现在开出的银币可是翻了5倍哟。
	add_talk #b#b
	option 163 #我愿用2000银币换一周陪伴布兰尼的机会！
	
	talk
endproc

;扣2000银币，给予1个粉色哨子
proc 163
	strvar ret
	get_id_player_info 0 14 ret
	if @ret >= 2000
		call 203
	else
		call 204
		add_sys_msg_start
		add_sys_msg 银币不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 203
	DEC_MONEY_TEST 0 2000 20 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY 0 2000 20 12
			if 12 = 0
				ADD_BIND_ITEM_NUM_INPAK 5241521 1 1 13
				add_sys_msg_start
				add_sys_msg 获得哨子
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 204
	call 1
endproc
