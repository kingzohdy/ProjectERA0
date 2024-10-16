

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一个人就想保卫时间城？很多事情不是靠热血就可以的，年轻人。试着建立或加入公会吧，团队和友谊比任何强大的装备更有意义。
	
	strvar RetTsk
	task_stat 3122 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	
	strvar RetTsk
	task_stat 4489 1 20
	if 20 = 2
		option 250 #向斯蒂芬斯赠送一个粽子
	endif
	
	strvar Ret2 SJzt
	GET_EVENT_STAT SJzt 65
	CLAN_EVERYDAY_PWORLD_STAT_GET Ret2
	
	if SJzt = 1 and Ret2 = 0
		option 160 #【驱逐入侵】活动（未开启）
	elseif  Ret2 = 1
		option 160 #【驱逐入侵】活动（已开启）
	elseif  SJzt = 1 and Ret2 = 2
		option 160 #【驱逐入侵】活动（已结束）
	else
	endif
	
	strvar Rets
	DYN_ACTIVE_STATUS_GET  1 19 Rets
	if Rets = 1
		option 500 #参加【公会宝藏】活动
	endif
	
	task_stat 3508 1 701
	task_stat 3509 1 702
	task_stat 3510 1 703
	task_stat 3511 1 704
	
	if 701 = 2 or 702 = 2 or 703 = 2 or 704 = 2
		option 750 #公会事务 - 上交物品
	endif
	
	task_add_option 1102 1 105
	task_add_option 3637 1 206
	task_add_option 3638 1 207
	task_add_option 3639 1 208
	task_add_option 3640 1 209
	task_add_option 3641 1 210
	task_add_option 3642 1 211
	task_add_option 3643 1 212
	task_add_option 3680 1 213
	
	TASKCYCLE_ADD_OPTION 108 1 120
	TASKCYCLE_ADD_OPTION 111 1 121
	TASKCYCLE_ADD_OPTION 115 1 122
	TASKCYCLE_ADD_OPTION 116 1 123
	TASKCYCLE_ADD_OPTION 117 1 124
	TASKCYCLE_ADD_OPTION 118 1 125
	
	
	TASKCYCLE_ADD_OPTION 123 1 140
	TASKCYCLE_ADD_OPTION 124 1 141
	TASKCYCLE_ADD_OPTION 125 1 142
	TASKCYCLE_ADD_OPTION 126 1 143
	TASKCYCLE_ADD_OPTION 127 1 144
	TASKCYCLE_ADD_OPTION 128 1 145
	TASKCYCLE_ADD_OPTION 129 1 146
	TASKCYCLE_ADD_OPTION 144 1 147
	
	strvar Ret1 Pos
	player_clan_pos_info Ret1 0 0 0 Pos
	
	strvar Ret
	GET_CLAN_BUILDING_LV Ret 0 5	
	if Ret = 0
		option 116 #申请公会城市
	endif
	if Ret > 0 and Ret < 100
		option 117 #进入公会城市
	endif
	
	if Ret1 = 0 and Pos = 0 
		option 110 创建公会
	endif
	
	option 112 #公会查询
	
	if Ret1 = 0 and Pos != 0
		strvar lv
		GET_PLAYER_BASE_ATT 0 0 lv 1
		if lv >= 40
			option 1000 我要立刻完成【公会事务】环组任务
		endif
	endif
	
	if Ret1 = 0 and Pos != 0 and Pos != 8
		option 115 退出公会	
	endif
	
	if Ret1 = 0 and Pos = 8
		option 113 #编辑宣告
		option 111 #公会解散
	endif
	
	talk
endproc 
proc 500
		GOTO_CLAN_CITY 1 12700 29100
endproc	

PROC 105
	task_do_option 1102 1
ENDPROC
PROC 206
	task_do_option 3637 1
ENDPROC
PROC 207
	task_do_option 3638 1
ENDPROC
PROC 208
	task_do_option 3639 1
ENDPROC
PROC 209
	task_do_option 3640 1
ENDPROC
PROC 210
	task_do_option 3641 1
ENDPROC
PROC 211
	task_do_option 3642 1
ENDPROC
PROC 212
	task_do_option 3643 1
ENDPROC
PROC 213
	task_do_option 3680 1
ENDPROC
; PROC 214
	; task_do_option 3508 1
; ENDPROC
; PROC 215
	; task_do_option 3509 1
; ENDPROC
; PROC 216
	; task_do_option 3510 1
; ENDPROC
; PROC 217
	; task_do_option 3511 1
; ENDPROC

proc 116
	 open_ui 18
	 strvar Lv
	 GET_CLAN_BUILDING_LV Lv 0 5
	 if Lv = 1
		EFFECT 2 0 SetMessageTips(id)
	 
	 endif
endproc

proc 117
	enter_clan_city	1
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每一年圣诞老公公都不会忘了送来圣诞礼物，尽管我已不再是孩童。
		option 201 #交出圣诞礼物
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3122 1
		endif
	endif
endproc 

PROC 120	
	TASKCYCLE_DO_OPTION 108 1
ENDPROC
PROC 121	
	TASKCYCLE_DO_OPTION 111 1
ENDPROC
PROC 122	
	TASKCYCLE_DO_OPTION 115 1
ENDPROC
PROC 123	
	TASKCYCLE_DO_OPTION 116 1
ENDPROC
PROC 124	
	TASKCYCLE_DO_OPTION 117 1
ENDPROC
PROC 125	
	TASKCYCLE_DO_OPTION 118 1
ENDPROC

PROC 140	
	TASKCYCLE_DO_OPTION 123 1
ENDPROC
PROC 141
	TASKCYCLE_DO_OPTION 124 1
ENDPROC
PROC 142	
	TASKCYCLE_DO_OPTION 125 1
ENDPROC
PROC 143	
	TASKCYCLE_DO_OPTION 126 1
ENDPROC
PROC 144	
	TASKCYCLE_DO_OPTION 127 1
ENDPROC
PROC 145	
	TASKCYCLE_DO_OPTION 128 1
ENDPROC
PROC 146	
	TASKCYCLE_DO_OPTION 129 1
ENDPROC
PROC 147	
	TASKCYCLE_DO_OPTION 144 1
ENDPROC


proc 115

	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《退会条约》
	add_talk #b#b#b 退会后全数返还当前剩余的#c186506贡献度#n
	add_talk #b 退会后返还#c18650680％#n学习公会技能花费的#c186506贡献度#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b自主退会
	add_talk #b#b#b 退会#c18650624小时#n后可申请加入任何公会
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b踢除退会
	add_talk #b#b#b#b退会后随时可申请加入任何公会
	add_talk #b#b#b #c005ebe（公会解散后视为自动踢除退会）#n
	
		option 300 #确定退出公会
		option 1 #返回

	talk
endproc 
proc 300
	choose 0 330 331 是否确认退出公会？
endproc

PROC 110
	DIALOG 1
	add_talk 建会花费：
	add_talk #b#b#c18650610000绑银
	add_talk #b#b原质矿×150个
	add_talk #b#b原生油×150个#n
	add_talk #b#b
	add_talk 建会提醒：
	add_talk 公会基本评议未达标，将自动回收解散公会
	add_talk #b#b[1-5]级公会的人数#c186506连续7天#n少于10人
	add_talk #b#b[1]级公会的资金#c186506连续15天#n不足结算维护费
	add_talk #b#b[1-5]级公会的成员#c186506连续60天#n无任何人在线

	option 332 #确定创建公会
	option 1 #返回
	talk
ENDPROC

proc 111
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b《散会条约》
	add_talk #b#b 会长自主申请解散，经确认后解散公会
	add_talk #b#b#b #c005ebe（需在申请后第48-96小时内确认）#n
	
		option 301 #确定解散公会
		option 1 #返回
	talk
endproc 

proc 330
	EXIT_CLAN
endproc
proc 332

	strvar num num1 
	GET_ITEM_NUM_INPAK 5101001 1 num
	GET_ITEM_NUM_INPAK 5102001 1 num1
	
	if num >= 150 and num1 >= 150

		; strvar ret
		; get_id_player_info 0 21 ret
		; get_id_player_info 0 22 30
		
		; if 30 < 10000 and @ret >= 10000
			; choose 1 333 331 绑银消耗已达到每日最大上限，创建公会时将会直接消耗银币，是否继续操作？
		; elseif ret < 10000
			
			; choose 1 333 331 绑银不足10000，创建公会成功后会消耗银币，是否继续操作？
		; else
			call 333
		; endif
		
		
	else
		add_sys_msg_start
		add_sys_msg 创建公会的材料不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
proc 333
	strvar num num1 
	GET_ITEM_NUM_INPAK 5101001 1 num
	GET_ITEM_NUM_INPAK 5102001 1 num1
	if num >= 150 and num1 >= 150
		open_ui 6
	else
		add_sys_msg_start
		add_sys_msg 创建公会的材料不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc 
PROC 301
	open_ui 7
ENDPROC
PROC 112
	open_ui 8
ENDPROC
PROC 113
	open_ui 9
ENDPROC

proc 2
	get_trig_task 1 2 
	if 2 = 3676
;		ADD_ITEM_NUM_INPAK 5300873 1 1 0
;		ADD_ITEM_NUM_INPAK 5300878 1 1 0
;		ADD_ITEM_NUM_INPAK 5300879 1 1 0
;		ADD_ITEM_NUM_INPAK 5300880 1 1 0
	endif

endproc


;驱逐入侵
proc 160
	strvar ret3 pos time ret ret2
	; PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	GET_EVENT_STAT ret 65
	CLAN_EVERYDAY_PWORLD_STAT_GET ret2

	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b自治联盟在一处资源丰富的海岛上秘密建立冶炼厂，作为公会重要的生产基地。但消息意外泄露到末世追随者耳中，为了抢夺资源，他们对冶炼岛发起了频繁的进攻。
	if ret = 1 and ret2 = 0
		add_talk #b#b现在急需召集公会成员协助驱逐末世追随者。铁壁已经在岛上等候着，它会告诉你们怎么做。
		add_talk #b#b
		add_talk #c186506开启要求：建立公会≥2天
		add_talk 参加要求：加入公会≥2天
		add_talk 活动时间：周二、周四各1次(06:00-24:00)
		add_talk 会长开启：06:00-22:00
		add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
		add_talk 活动进展：未开启
		add_talk 持续时间：活动持续20分钟#n

			if time >= 6 and time < 24
					option 400 #我已召集成员，开启【驱逐入侵】活动
			endif
	elseif ret2 = 1
			add_talk #b#b准备好后就找我传送到公会冶炼岛吧，铁壁已经在岛上等候着，它会告诉你们该怎么做。
			add_talk #b#b
			add_talk #b#b#c005ebe你可以通过快捷栏上方的“公会活动助手”按钮兑换积分奖励、查询规则与剩余时间。#n
			add_talk #b#b
			add_talk #c186506参加要求：加入公会≥2天
			add_talk 活动时间：周二、周四各1次(06:00-24:00)
			add_talk 活动进展：进行中
			add_talk 持续时间：活动持续20分钟#n

			option 401 #火速前往【公会冶炼岛】
	elseif ret = 1 and ret2 = 2
			add_talk #b#b经过大家的携手奋战，今天我们再次击退入侵者，成功捍卫了我们的尊严！
			add_talk #b#b
			add_talk #c186506活动时间：周二、周四各1次(06:00-24:00)
			add_talk 会长开启：06:00-22:00
			add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
			add_talk 活动进展：已结束#n
	else
	endif
	
		option 1 返回
	talk
endproc

proc 400
	strvar ret time ret2 ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【驱逐入侵】已开启，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【驱逐入侵】已结束，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg 加入公会当天，无法开启【驱逐入侵】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【驱逐入侵】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【驱逐入侵】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 402 403 是否确认开启【驱逐入侵】活动？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【驱逐入侵】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 402 403 是否确认开启【驱逐入侵】活动？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【驱逐入侵】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif	
endproc

proc 402
	strvar ret time ret2 ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【驱逐入侵】已开启，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【驱逐入侵】已结束，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg 加入公会当天，无法开启【驱逐入侵】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【驱逐入侵】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【驱逐入侵】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			CLAN_EVERYDAY_PWORLD_CREATE 0 @pos
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【驱逐入侵】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			CLAN_EVERYDAY_PWORLD_CREATE 0 @pos
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【驱逐入侵】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 401
	strvar ret ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4
	
	if  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【驱逐入侵】已结束，今天无法再参加活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg 加入公会当天，无法参加【驱逐入侵】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动时间内，无法参加【驱逐入侵】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 24
		if ret3 = 0 and pos > 0
			CLAN_EVERYDAY_PWORLD_ENTER 0
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法参加【驱逐入侵】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b前些时候练习枪法的时候，不慎走火，失了大部分的珍藏，其实就包括这本公会志，你来的真是时候。
	option 601 #交换
		
	talk
endproc
proc 601
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300775 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300775 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300776 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300777 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300778 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300779 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
;按照等级打开我很忙界面
proc 1000
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv >= 40 and lv <= 49
		PLAYER_BUSY_UI 24 1
	endif
	if lv >= 50 and lv <= 59
		PLAYER_BUSY_UI 25 1
	endif
	if lv >= 60 and lv <= 69
		PLAYER_BUSY_UI 26 1
	endif
	if lv >= 70 and lv <= 79
		PLAYER_BUSY_UI 27 1
	endif
	if lv >= 80 and lv <= 89
		PLAYER_BUSY_UI 28 1
	endif
endproc


proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4489 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4489 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4489 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 


proc 502
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

proc 750
	dialog 3
	add_talk $_npc$:
	add_talk #b#b把你带来的物品上交给我，看看是否我所需要的物品。
	add_talk #b#b
	
	task_stat 3508 1 701
	task_stat 3509 1 702
	task_stat 3510 1 703
	task_stat 3511 1 704
	
	if 701 = 2
		add_talk #b#b#c186506需要上交1个称号的头衔#n
	endif
	if 702 = 2
		add_talk #b#b#c186506需要上交1张科技生产的配方#n
	endif
	if 703 = 2
		add_talk #b#b#c186506需要上交1张科技生产的配方#n
	endif
	if 704 = 2
		add_talk #b#b#c186506需要上交1张科技生产的配方#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c005ebe将上交的物品拖至下方的空槽内#n
		
	NPC_COMMIT_OPT 751
	talk
endproc

proc 751
	strvar itemID pos index ret
	GET_COMMIT_ITEM_INFO 1 itemID
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	if itemID = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	task_stat 3508 1 701
	if 701 = 2
	ITEM_IS_CHENGHAO 1 @itemID
		if 1 = -1
			add_sys_msg_start
			add_sys_msg 我只需要称号的头衔，任何一个称号头衔都可以哟！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		if 1 = 0
		DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3508 0
					call 752
				endif
			endif
		endif
	endif

	task_stat 3509 1 703
	if 703 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3509 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3509 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3509 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
	task_stat 3510 1 703
	if 703 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3510 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3510 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3510 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
	task_stat 3511 1 704
	if 704 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3511 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3511 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg 我只需要科技生产的配方，任何一项科技的配方都可以哟！
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3511 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
endproc

proc 752
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 753
	add_sys_msg_start
	add_sys_msg 物品不足要求数量，未能成功上交
	set_sys_msg_mode 3 0 0
	msg_info
endproc
