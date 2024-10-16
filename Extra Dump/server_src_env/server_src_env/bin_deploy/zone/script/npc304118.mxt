proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b勤劳的勇士为自由联盟的繁荣做出了不可磨灭的贡献，值此佳节，大行政官特意准备了丰富的礼品派我来发送，祝各位勇士身体健康、吉祥如意！
	add_talk #b#b#c005ebe5月6日，艾劳就会离开时间城，到偏远地区进行慰问工作！
	add_talk #b#b#c186506欢乐致富之礼（嘉年华宝箱获得的银币翻5倍）
	
	OPTION 300 慰问工作任务（节日特别任务）
	OPTION 100 普天同庆之礼（领取在线奖励）
	OPTION 200 无上荣誉之礼（战魂徽章兑换双倍荣誉）
	
	talk
endproc

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b就在大家欢庆节日时，我们身边仍有许许多多劳动人们奋斗在第一线，让我们献出一点爱，将感谢的心送到他们怀里。
	
	task_add_option 4440 1 400
	task_add_option 4441 1 401
	task_add_option 4442 1 402
	task_add_option 4443 1 403
	task_add_option 4444 1 404
	task_add_option 4445 1 405
	
	TASKCYCLE_ADD_OPTION 137 1 440
	
	OPTION 1 返回
	
	talk
endproc

proc 100	
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

	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每天早上6点重置在线时间，当天在线时间可累计。
	add_talk #b#b您今日在线时间为：$@10$时$@11$分
	GET_PLAYER_DAILY_FLAG 0 9 20
	GET_PLAYER_DAILY_FLAG 0 10 21
	GET_PLAYER_DAILY_FLAG 0 11 22
	if 20 = 0
	OPTION 101 领取在线1小时奖励
	endif
	
	if 21 = 0
	OPTION 102 领取在线2小时奖励
	endif
	
	if 22 = 0
	OPTION 103 领取在线4小时奖励
	endif
	
	option 1 返回
	talk
endproc

proc 101
	GET_DAYONLINE 1 10

	if 10 < 3600
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，您今日在线时间不足1小时，不能领取奖励。
		option 100 返回
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 9 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 30
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 102
	GET_DAYONLINE 1 10

	if 10 < 7200
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，您今日在线时间不足2小时，不能领取奖励。
		option 100 返回
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 10 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 50
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 103
	GET_DAYONLINE 1 10

	if 10 < 14400
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，您今日在线时间不足4小时，不能领取奖励。
		option 100 返回
		talk
	else
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 11 1
		if Ret = 0
			GET_ID_PLAYER_INFO 0 19 11
			DEV 11 10000
			MUL 11 100
			
			ADD_EXP 0 @11 12
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	; add_talk #b#b这是大行政官特意为奋斗在战场中的勇士们准备的豪华大礼，每天你都可以在我这里用战场牌子兑换荣誉，无论是战魂徽章还是生存奖章，都可以兑换到1000的荣誉。
	add_talk #b#b这是大行政官特意为奋斗在战场中的勇士们准备的豪华大礼，每天你都可以在我这里用战魂徽章兑换荣誉。
	add_talk #b
	add_talk #b #c005ebe每次可兑换1000荣誉，活动总共可兑换18次
	; add_talk #b#b#cff4e00生存奖章#n通过参与#cff4e00狂沙角斗场#n获得
	add_talk #b#b#b#b战魂徽章通过参与绿洲风暴获得
	add_talk #b#b荣誉商人巴菲提供用荣誉换取奖章的服务#n
	
	; GET_PLAYER_DAILY_FLAG 0 13 20
	; if 20 = 0
	strvar zhi
	GET_ROLE_VAR 73 8 0 zhi
	add_talk #b
	add_talk #b#b#b#b#b#b 已成功兑换：#c186506$@zhi$次#n
	
	if zhi >= 18
		add_talk #b#b#b #c186506您已全部兑换18次获得18000荣誉#n
	endif
	if zhi < 18
		OPTION 201 1枚战魂徽章 兑换1000荣誉
	endif
		; OPTION 202 1枚生存奖章 兑换 1000荣誉
	; else
		; OPTION 210 #您今天已经成功兑换过荣誉。
	; endif
	option 1 返回
	talk
endproc

proc 201
	strvar zhi
	GET_ROLE_VAR 73 8 0 zhi
	
	if zhi < 18
		GET_ITEM_NUM_INPAK 5060003 0 11
		if 11 > 0
			DEC_ITEM_NUM_INPAK 5060003 1 12
			if 12 = 0
				; strvar Ret 
				; SET_PLAYER_DAILY_FLAG Ret 13 1
				ADD_MONEY 0 1000 76 Ret
				if Ret = 0
					add zhi 1
					SET_ROLE_VAR 73 8 @zhi 0
				endif
			endif
			PLAYER_CLOSE_DIALOG1 0 0
		else
			GET_ID_PLAYER_INFO 0 24 13
			if 13 > 0
				; SET_PLAYER_DAILY_FLAG 0 13 1
				strvar ret2
				DEC_MONEY 0 1 77 ret2
				if ret2 = 0
					ADD_MONEY 0 1000 76 Ret
					if Ret = 0
						add zhi 1
						SET_ROLE_VAR 73 8 @zhi 0
					endif
				endif
				PLAYER_CLOSE_DIALOG1 0 0
			else
				dialog 1
				add_talk $_npc$:
				add_talk #b#b您的奖章中并未持有战魂徽章，暂时无法兑换荣誉。
				option 200 返回
				talk
			endif
		endif
	endif
endproc

proc 202
	GET_ITEM_NUM_INPAK 5060009 0 11
	if 11 > 0
		DEC_ITEM_NUM_INPAK 5060009 1 12
		if 12 = 0
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 13 1
			ADD_MONEY 0 1000 76 0
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	else
		GET_ID_PLAYER_INFO 0 25 13
		if 13 > 0
			DEC_MONEY 0 1 78 0
			SET_PLAYER_DAILY_FLAG 0 13 1
			ADD_MONEY 0 1000 76 0
			PLAYER_CLOSE_DIALOG1 0 0
		else
			dialog 1
			add_talk $_npc$:
			add_talk #b#b对不起，您的背包里还没有生存奖章。
			option 200 返回
			talk
		endif
	endif
endproc

proc 400
	task_do_option 4440 1
endproc

proc 401
	task_do_option 4441 1
endproc

proc 402
	task_do_option 4442 1
endproc

proc 403
	task_do_option 4443 1
endproc

proc 404
	task_do_option 4444 1
endproc

proc 405
	task_do_option 4445 1
endproc

proc 440
	TASKCYCLE_DO_OPTION 137 1
endproc
