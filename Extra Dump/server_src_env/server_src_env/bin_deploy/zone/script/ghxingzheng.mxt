;公会守护枢纽活动    ,公会行政
proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b是朋友还是敌人？这是个问题……
		add_talk #b#b我是城市行政长官$_npc$，我代表【#c186506$_CityOwnerClanName$#n】公会的全体成员欢迎您的到来！
		add_talk #b#b
		add_talk #b#b您现在可以在【#c186506$_CityOwnerClanName$#n】公会的城市里逛一下了，不过要注意我们的卫兵哦，如果惹到他们可是会很惨的啊……
	else
	
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0	
	GET_CLAN_BUILDING_LV RetC 0 1
	GET_CLAN_BUILDING_LV RetG 0 2
	GET_CLAN_BUILDING_LV RetCh 0 5
		
	GET_CLAN_USED_TECH_POINT ZRet 0 0
	
	add_talk $_npc$:
	add_talk #b#b你能见到我说明你已经很强大了，不过后边还有很多的事情等着你去探索。
;	add_talk #b#b#c186506当前公会城市等级为：$@RetCh$级#n
	if Ret = 100 or Ret = 0
		add_talk #b#b#c186506当前科技中心等级为：0级（未建设）#n
	endif
	if Ret = 1 or Ret = 101
		add_talk #b#b#c186506当前科技中心等级为：1级#n
	endif
	if Ret = 2 or Ret = 102
		add_talk #b#b#c186506当前科技中心等级为：2级#n
	endif
;;;;;;;;;;;;;;;;;;;;;;;;
	if RetC = 100 or RetC = 0
;		add_talk #b#b#c186506当前采集中心等级为：0级（未建设）#n
	endif
	if RetC = 1 or RetC = 101
;		add_talk #b#b#c186506当前采集中心等级为：1级#n
	endif
	if RetC = 2 or RetC = 102
;		add_talk #b#b#c186506当前采集中心等级为：2级#n
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;
	if RetG = 100 or RetG = 0
;		add_talk #b#b#c186506当前工厂等级为：0级（未建设）#n
	endif
	if RetG = 101 or RetG = 1
;		add_talk #b#b#c186506当前工厂等级为：1级#n
	endif
	if RetG = 102 or RetG = 2
;		add_talk #b#b#c186506当前工厂等级为：2级#n
	endif
	
	add_talk #b#b
;	add_talk #b#b#c186506当前公会投入的总科技点为：$@ZRet$#n	
		
;	TASKCYCLE_ADD_OPTION 110 1 133
;	TASKCYCLE_ADD_OPTION 119 1 160
;	TASKCYCLE_ADD_OPTION 120 1 161
;	TASKCYCLE_ADD_OPTION 112 1 162
;	TASKCYCLE_ADD_OPTION 113 1 163
;	TASKCYCLE_ADD_OPTION 114 1 164
	
;;;;;;;;;;;;;;;;;;;;;;;;;;30~40↓	
	
	strvar Ret3227 Ret3228 Ret3229 Ret3230 Ret3231 Ret3232 Ret3233 Ret3234 Ret3235 Ret3236
	task_stat 3227 1 Ret3227
	task_stat 3228 1 Ret3228
	task_stat 3229 1 Ret3229
	task_stat 3230 1 Ret3230
	task_stat 3231 1 Ret3231
	task_stat 3232 1 Ret3232
	task_stat 3233 1 Ret3233
	task_stat 3234 1 Ret3234
	task_stat 3235 1 Ret3235
	task_stat 3236 1 Ret3236
	if Ret3227 = 2 or Ret3228 = 2 or Ret3229 = 2 or Ret3230 = 2 or Ret3231 = 2 or Ret3232 = 2 or Ret3233 = 2 or Ret3234 = 2 or Ret3235 = 2 or Ret3236 = 2 
		option 135 #公会任务-上交物品
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;41~50↓
	strvar Ret3241 Ret3242 Ret3243 Ret3244 Ret3245 Ret3246 Ret3247 Ret3248 Ret3249 Ret3250
	task_stat 3241 1 Ret3241
	task_stat 3242 1 Ret3242
	task_stat 3243 1 Ret3243
	task_stat 3244 1 Ret3244
	task_stat 3245 1 Ret3245
	task_stat 3246 1 Ret3246
	task_stat 3247 1 Ret3247
	task_stat 3248 1 Ret3248
	task_stat 3249 1 Ret3249
	task_stat 3250 1 Ret3250
	if Ret3241 = 2 or Ret3242 = 2 or Ret3243 = 2 or Ret3244 = 2 or Ret3245 = 2 or Ret3246 = 2 or Ret3248 = 2 or Ret3249 = 2 or Ret3250 = 2 or Ret3247 = 2 
		option 136 #公会任务-上交物品
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;50~60↓
	strvar Ret3259 Ret3260 Ret3261 Ret3262 Ret3263 Ret3264 Ret3265 Ret3266 Ret3267 Ret3268
	task_stat 3259 1 Ret3259
	task_stat 3260 1 Ret3260
	task_stat 3261 1 Ret3261
	task_stat 3262 1 Ret3262
	task_stat 3263 1 Ret3263
	task_stat 3264 1 Ret3264
	task_stat 3265 1 Ret3265
	task_stat 3266 1 Ret3266
	task_stat 3267 1 Ret3267
	task_stat 3268 1 Ret3268
	if Ret3259 = 2 or Ret3260 = 2 or Ret3261 = 2 or Ret3262 = 2 or Ret3263 = 2 or Ret3264 = 2 or Ret3265 = 2 or Ret3266 = 2 or Ret3267 = 2 or Ret3268 = 2 
		option 146 #公会任务-上交物品
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;60~70↓
	strvar Ret3569 Ret3570 Ret3571 Ret3572 Ret3573 Ret3574 Ret3575 Ret3576 Ret3577 Ret3578
	task_stat 3569 1 Ret3569
	task_stat 3570 1 Ret3570
	task_stat 3571 1 Ret3571
	task_stat 3572 1 Ret3572
	task_stat 3573 1 Ret3573
	task_stat 3574 1 Ret3574
	task_stat 3575 1 Ret3575
	task_stat 3576 1 Ret3576
	task_stat 3577 1 Ret3577
	task_stat 3578 1 Ret3578
	if Ret3569 = 2 or Ret3570 = 2 or Ret3571 = 2 or Ret3572 = 2 or Ret3573 = 2 or Ret3574 = 2 or Ret3575 = 2 or Ret3576 = 2 or Ret3577 = 2 or Ret3578 = 2 
		option 156 #公会任务-上交物品
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;70~80↓
	strvar Ret3617 Ret3618 Ret3619 Ret3620 Ret3621 Ret3622 Ret3623 Ret3624 Ret3625 Ret3626
	task_stat 3617 1 Ret3617
	task_stat 3618 1 Ret3618
	task_stat 3619 1 Ret3619
	task_stat 3620 1 Ret3620
	task_stat 3621 1 Ret3621
	task_stat 3622 1 Ret3622
	task_stat 3623 1 Ret3623
	task_stat 3624 1 Ret3624
	task_stat 3625 1 Ret3625
	task_stat 3626 1 Ret3626
	if Ret3617 = 2 or Ret3618 = 2 or Ret3619 = 2 or Ret3620 = 2 or Ret3621 = 2 or Ret3622 = 2 or Ret3623 = 2 or Ret3624 = 2 or Ret3625 = 2 or Ret3626 = 2 
		option 166 #公会任务-上交物品
	endif	
	
	if Ret = 0
	
		option 121 #建设科技中心	
	endif
;	if RetC = 0 and Ret != 0
;		option 125 #建设采集中心
;	endif
	if RetG = 0 and Ret != 0
		option 124 #建设工厂
	endif
	
;	if RetCh < 2
;		option 122 #升级城市
;	endif

	strvar judian kaiqi zhuangtai moshi shijian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	
	STRONG_ACTIVE_LEVEL judian
	IS_EXIST_MACHINE_PWORLD_UINTE kaiqi 0
	
	if shijian = 1 and zhuangtai = 0
		option 300 #【守护公会枢纽-普通模式】（未开启）
		if judian > 0
			option 500 #【守护公会枢纽-联合模式】（未开启）
		endif
	endif
	
	if kaiqi = 1
		if zhuangtai = 1 and moshi = 1
			option 450 #【守护公会枢纽-普通模式】（未开启）
			option 500 #【守护公会枢纽-联合模式】（本会进行中）
		else
			option 500 #【守护公会枢纽-联合模式】（外会进行中）
		endif
	endif
	if kaiqi = 1
		if zhuangtai = 1 and moshi != 1
			option 300 #【守护公会枢纽-普通模式】（进行中）
			if judian > 0
				option 451 #【守护公会枢纽-联合模式】（未开启）
			endif
		endif
	endif
	
	if shijian = 1 and zhuangtai = 2
		option 452 #【守护公会枢纽-普通模式】（已结束）
		if judian > 0
			option 453 #【守护公会枢纽-联合模式】（已结束）
		endif
	endif
	
;	option 120 #回到时间城
	endif
;	option 210 公会信息查询
	talk
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 210
	
	EFFECT 2 1 SetMessageTips(1)
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 136
	exec ghtask40_50.mac 100
endproc 
proc 137
	exec ghtask40_50.mac 111
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;50~60
proc 146
	exec ghtask50_60.mac 150
endproc 
proc 147
	exec ghtask50_60.mac 151
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;60~70
proc 156
	exec ghtask60_70.mac 160
endproc
proc 157
	exec ghtask60_70.mac 161
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;70~80
proc 166
	exec ghtask70_80.mac 170
endproc
proc 167
	exec ghtask70_80.mac 171
endproc
proc 120
	TRANSMIT 1000 25200 39000 1
endproc 
proc 121
	open_ui 21
endproc
proc 122
	open_ui 18
endproc
proc 125
	open_ui 22
endproc	
proc 124
	open_ui 23
endproc
PROC 126
	TASKCYCLE_DO_OPTION 110 1
ENDPROC
PROC 160
	TASKCYCLE_DO_OPTION 119 1
ENDPROC
PROC 161
	TASKCYCLE_DO_OPTION 120 1
ENDPROC
PROC 162
	TASKCYCLE_DO_OPTION 112 1
ENDPROC
PROC 163
	TASKCYCLE_DO_OPTION 113 1
ENDPROC
PROC 164
	TASKCYCLE_DO_OPTION 114 1
ENDPROC
proc 135
	dialog 3
	add_talk $_npc$:
	add_talk #b#b收集回来的东西交给我先审视。
	add_talk #b#b
	strvar Ret3227 Ret3228 Ret3229 Ret3230 Ret3231 Ret3232 Ret3233 Ret3234 Ret3235 Ret3236
	
	task_stat 3227 1 Ret3227
	task_stat 3228 1 Ret3228
	task_stat 3229 1 Ret3229
	task_stat 3230 1 Ret3230
	task_stat 3231 1 Ret3231
	task_stat 3232 1 Ret3232
	task_stat 3233 1 Ret3233
	task_stat 3234 1 Ret3234
	task_stat 3235 1 Ret3235
	task_stat 3236 1 Ret3236
			        
	if Ret3227 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质主手物品#n
	endif
	if Ret3228 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质副手物品#n
	endif
	if Ret3229 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质头盔#n
	endif
	if Ret3230 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质肩甲#n
	endif
	if Ret3231 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质面饰#n
	endif
	if Ret3232 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质胸甲#n
	endif	
	if Ret3233 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质战靴#n
	endif
	if Ret3234 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质项链#n
	endif
	if Ret3235 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质戒指#n
	endif
	if Ret3236 = 2
		add_talk #b#b#c186506需要收集30级~40级阶段的绿色品质护符#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c186506请把任务需要上交的物品放到下方方槽处。#n
	add_talk #b#b#c186506<右键点击背包内物品也可直接放入>#n
	
		NPC_COMMIT_OPT 200
	talk
endproc 
proc 200	
		strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
		GET_COMMIT_ITEM_INFO 1 m_ID
		GET_COMMIT_ITEM_INFO 2 m_Pos1
		GET_COMMIT_ITEM_INFO 3 m_Pos2
		GET_COMMIT_ITEM_INFO 4 m_Col
		if m_ID = -1
			add_sys_msg_start
			add_sys_msg 你还没有提交物品！
			set_sys_msg_mode 1 0 0
			msg_info		
			return 
		endif
		
		task_stat 3227 1 Ret
		if Ret = 2
			if m_ID = 1013004 or m_ID = 1253004 or m_ID = 1203004 or m_ID = 1103004 or m_ID = 1033004 or m_ID = 1503004
				switch m_ID
					case 1013004
						DEC_ITEM_NUM_BYIDX 1013004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1253004
						DEC_ITEM_NUM_BYIDX 1253004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1203004
						DEC_ITEM_NUM_BYIDX 1203004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1103004
						DEC_ITEM_NUM_BYIDX 1103004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1033004
						DEC_ITEM_NUM_BYIDX 1033004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1503004
						DEC_ITEM_NUM_BYIDX 1503004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
				
			endif
			
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3228 1 Ret
		if Ret = 2
			if m_ID = 1583004 or m_ID = 1283004 or m_ID = 1183004 or m_ID = 1063004 
				switch m_ID
					case 1583004
						DEC_ITEM_NUM_BYIDX 1583004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1283004
						DEC_ITEM_NUM_BYIDX 1283004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1183004
						DEC_ITEM_NUM_BYIDX 1183004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1063004
						DEC_ITEM_NUM_BYIDX 1063004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3229 1 Ret
		if Ret = 2
			if m_ID = 2133004 or m_ID = 2143004 or m_ID = 2123004 or m_ID = 2113004 
				switch m_ID
					case 2133004
						DEC_ITEM_NUM_BYIDX 2133004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2143004
						DEC_ITEM_NUM_BYIDX 2143004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2123004
						DEC_ITEM_NUM_BYIDX 2123004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2113004
						DEC_ITEM_NUM_BYIDX 2113004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3230 1 Ret
		if Ret = 2
			if m_ID = 2243004 or m_ID = 2223004 or m_ID = 2213004 or m_ID = 1273004 
				switch m_ID
					case 2243004
						DEC_ITEM_NUM_BYIDX 2243004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 2223004
						DEC_ITEM_NUM_BYIDX 2223004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 2213004
						DEC_ITEM_NUM_BYIDX 2213004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 1273004
						DEC_ITEM_NUM_BYIDX 1273004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3231 1 Ret
		if Ret = 2
			if m_ID = 2423004 or m_ID = 2433004 or m_ID = 2443004 or m_ID = 2413004 
				switch m_ID
					case 2423004
						DEC_ITEM_NUM_BYIDX 2423004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2433004
						DEC_ITEM_NUM_BYIDX 2433004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2443004
						DEC_ITEM_NUM_BYIDX 2443004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2413004
						DEC_ITEM_NUM_BYIDX 2413004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3232 1 Ret
		if Ret = 2
			if m_ID = 2013004 or m_ID = 2023004 or m_ID = 2033004 or m_ID = 2043004 
				switch m_ID
					case 2013004
						DEC_ITEM_NUM_BYIDX 2013004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2023004
						DEC_ITEM_NUM_BYIDX 2023004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2033004
						DEC_ITEM_NUM_BYIDX 2033004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2043004
						DEC_ITEM_NUM_BYIDX 2043004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3233 1 Ret
		if Ret = 2
			if m_ID = 2613004 or m_ID = 2633004 or m_ID = 2643004 or m_ID = 2623004 
				switch m_ID
					case 2613004
						DEC_ITEM_NUM_BYIDX 2613004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2633004
						DEC_ITEM_NUM_BYIDX 2633004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2643004
						DEC_ITEM_NUM_BYIDX 2643004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2623004
						DEC_ITEM_NUM_BYIDX 2623004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3234 1 Ret
		if Ret = 2
		
			if m_ID = 2813004 or m_ID = 2833004 or m_ID = 2843004 or m_ID = 2823004 
				switch m_ID
					case 2813004
						DEC_ITEM_NUM_BYIDX 2813004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2833004
						DEC_ITEM_NUM_BYIDX 2833004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2843004
						DEC_ITEM_NUM_BYIDX 2843004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2823004
						DEC_ITEM_NUM_BYIDX 2823004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3235 1 Ret
		if Ret = 2
			if m_ID = 2933004 or m_ID = 2923004 or m_ID = 2913004 or m_ID = 2943004 
				switch m_ID
					case 2933004
						DEC_ITEM_NUM_BYIDX 2933004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2923004
						DEC_ITEM_NUM_BYIDX 2923004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2913004
						DEC_ITEM_NUM_BYIDX 2913004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2943004
						DEC_ITEM_NUM_BYIDX 2943004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
				
				endswitch
			else
				add_sys_msg_start
				add_sys_msg 物品不符任务要求，未能成功上交
				set_sys_msg_mode 1 0 0
				msg_info
			endif
		endif
;;;;;;;;;		;;;;;;;;
		task_stat 3236 1 Ret
		if Ret = 2
			if m_ID = 2713004 or m_ID = 2723004 or m_ID = 2733004 or m_ID = 2743004 
				switch m_ID
					case 2713004
						DEC_ITEM_NUM_BYIDX 2713004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2723004
						DEC_ITEM_NUM_BYIDX 2723004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2733004
						DEC_ITEM_NUM_BYIDX 2733004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2743004
						DEC_ITEM_NUM_BYIDX 2743004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
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
proc 130
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 
proc 300
	strvar ret ret2 ret3 pos time ope
	GET_EVENT_STAT ret 64
	GET_MACHINE_PWORLD_STATUS ret2 0
	get_now_time 1 1 1 1 time 1 1
	IS_CLAN_MACHINE_PWORLD_OPEN ope
	
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
;	add_talk $_npc$:
	add_talk 【普通模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	if ret = 1 and ret2 = 0
		add_talk #b#b
		if judian > 0
			add_talk #c186506开启要求：未开启【守护公会枢纽-联合模式】
			add_talk 参加要求：未参加任何模式的【守护公会枢纽】#n
		endif
		add_talk #c186506活动时间：每周六1次(06:00-24:00)
		add_talk 会长开启：06:00-22:00
		add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
		add_talk 活动进展：未开启#n
		add_talk #b#b
		add_talk #b#b#c005ebe使用机械可对敌方造成较高的伤害
		add_talk #b#b需要齐心协力才能成功击退所有敌人#n

		if time >= 6 and time < 24
			if ope = 0
				option 301 #我已召集会员，开启【守护公会枢纽-普通模式】
			endif
		endif
	elseif ret2 = 1
		add_talk #b#b
		if judian > 0
			add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】#n
		endif
		add_talk #c186506活动时间：每周六1次(06:00-24:00)
		add_talk 活动进展：进行中#n
		add_talk #b#b
		add_talk #b#b#c005ebe使用机械可对敌方造成较高的伤害
		add_talk #b#b需要齐心协力才能成功击退所有敌人#n
			
		if ope = 1
			option 400 #火速前往【公会枢纽大厅-普通模式】
		endif
	else
	endif
	
	option 1 #返回
	talk
endproc
proc 400
	strvar get ret2 pos time

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if  get = 2
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-普通模式】已结束，今天无法再参加
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg 不在开启时间内，无法参加【守护公会枢纽-普通模式】 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 24
		if ret2 = 0 and pos > 0
			ENTER_CLAN_MACHINE_PWORLD 1
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法参加【守护公会枢纽-普通模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 301
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动 
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	
	strvar get ret2 pos time

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if  get = 1
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-普通模式】已开启，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  get = 2
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-普通模式】已结束，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg 不在开启时间内，无法开启【守护公会枢纽-普通模式】
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			choose 2 302 303 是否确认开启【守护公会枢纽-普通模式】？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【守护公会枢纽-普通模式】
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			choose 2 302 303 是否确认开启【守护公会枢纽-普通模式】？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【守护公会枢纽-普通模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 302
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar get ret2 pos time ret3

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if get = 1
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-普通模式】已开启，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif get = 2
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-普通模式】已结束，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg 不在开启时间内，无法开启【守护公会枢纽-普通模式】
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			CLAN_MACHINE_PWORLD_CREATE ret3 0
			if ret3 = -1
				add_sys_msg_start
				add_sys_msg 已开启过【守护公会枢纽-普通模式】，每周最多只能开启1次  
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = -2
				add_sys_msg_start
				add_sys_msg 建立公会的第一周，无法开启【守护公会枢纽-普通模式】
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = 0
				add_sys_msg_start
				add_sys_msg 【$_Player$】开启了【守护公会枢纽-普通模式】，可通过#L公会城市@@(1000,25400,39200)#n的#L克利斯提尼@@(60000,17700,25500)#n传送到守护枢纽大厅协助作战
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			elseif ret3 = -3
				add_sys_msg_start
				add_sys_msg 同时只能1个公会开启【守护公会枢纽-联合模式】，请结束后再开启
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			else
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【守护公会枢纽-普通模式】
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			CLAN_MACHINE_PWORLD_CREATE ret3 0
			if ret3 = -1
				add_sys_msg_start
				add_sys_msg 已开启过【守护公会枢纽-普通模式】，每周最多只能开启1次  
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = -2
				add_sys_msg_start
				add_sys_msg 建立公会的第一周，无法开启【守护公会枢纽-普通模式】
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = 0
				add_sys_msg_start
				add_sys_msg 【$_Player$】开启了【守护公会枢纽-普通模式】，可通过#L公会城市@@(1000,25400,39200)#n的#L克利斯提尼@@(60000,17700,25500)#n传送到守护枢纽大厅协助作战
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			elseif ret3 = -3
				add_sys_msg_start
				add_sys_msg 同时只能1个公会开启【守护公会枢纽-联合模式】活动，请结束后再开启
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			else
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【守护公会枢纽-普通模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 303
endproc

proc 500
	strvar time kaiqi yinbi
	get_now_time 1 1 1 1 time 1 1
	IS_EXIST_MACHINE_PWORLD_UINTE kaiqi 0
	
	DIALOG 1
;	add_talk $_npc$:
	add_talk 【联合模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	add_talk #c005ebe#b#b4.击杀敌兵获得经验值增加10%
	add_talk #b#b5.任何公会的成员都可参与活动
	add_talk #b#b6.开启的公会可获得20000公会资金#n
	
	if kaiqi != 1
		add_talk #b#b
		add_talk #c186506开启要求：未开启【守护公会枢纽-普通模式】
		add_talk 参加要求：未参加任何模式的【守护公会枢纽】
		add_talk 活动时间：每周六1次(06:00-24:00)
		add_talk 会长开启：06:00-22:00
		add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
		add_talk 活动进展：未开启#n
		
		GET_ID_PLAYER_INFO 0 14 yinbi
		if yinbi < 50000
			add_talk #c186506开启费用：#n#cF80B0B $@yinbi$ #n#c186506/50000银币#n
		else
			add_talk #c186506开启费用：$@yinbi$ /50000银币#n			
		endif
			
		if time >= 6 and time < 24
			option 501 #我已召集会员，开启【守护公会枢纽-联合模式】
		endif
	elseif kaiqi = 1
		add_talk #b#b
		add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】
		add_talk 活动时间：每周六1次(06:00-24:00)
		add_talk 活动进展：进行中#n
		
		option 600 #火速前往【公会枢纽大厅-联合模式】
	else
	endif
	
	option 1 #返回
	talk
endproc

proc 600
	strvar ret2 pos time

	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if time < 6
		add_sys_msg_start
		add_sys_msg 不在时间内，无法参加【守护公会枢纽-联合模式】
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 24
		if ret2 = 0 and pos > 0
			ENTER_CLAN_MACHINE_PWORLD_UNITE 0
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法参加【守护公会枢纽-联合模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 501
	strvar Ret RetG
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动 
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	
	strvar get ret2 pos time fanrong
	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	RANGE_CLAN_FANRONG fanrong 0

	if fanrong > 6
		add_sys_msg_start
		add_sys_msg 繁荣度排名前6的公会可开启【守护公会枢纽-联合模式】
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  get = 1
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-联合模式】已开启，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg 不在开启时间内，无法开启【守护公会枢纽-联合模式】
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			choose 2 502 503 开启【守护公会枢纽-联合模式】需要支付50000银币  是否确认开启？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【守护公会枢纽-联合模式】
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			choose 2 502 503 开启【守护公会枢纽-联合模式】需要支付50000银币  是否确认开启？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【守护公会枢纽-联合模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 502
	strvar Ret RetG
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg 公会科技中心或公会工厂还未建造，无法开启活动  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar get ret2 pos time ret3 yinbi biaoji fanrong

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	GET_ID_PLAYER_INFO 0 14 yinbi
	RANGE_CLAN_FANRONG fanrong 0

	if fanrong > 6
		add_sys_msg_start
		add_sys_msg 繁荣度排名前6的公会可开启【守护公会枢纽-联合模式】
		set_sys_msg_mode 4 0 0
		msg_info
	elseif get = 1
		add_sys_msg_start
		add_sys_msg 【守护公会枢纽-联合模式】已开启，今天无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg 不在开启时间内，无法开启【守护公会枢纽-联合模式】
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			if yinbi >= 50000
				CLAN_MACHINE_PWORLD_CREATE ret3 1
				if ret3 = -1
					add_sys_msg_start
					add_sys_msg 已开启过【守护公会枢纽-联合模式】，每周最多只能开启1次  
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = -2
					add_sys_msg_start
					add_sys_msg 建立公会的第一周，无法开启【守护公会枢纽-联合模式】
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = 0
					DEC_MONEY 0 50000 20 biaoji
					if biaoji = 0
						ADD_ID_CLAN_MONEY 0 0 20000 1 0
					endif

					
					add_sys_msg_start
					add_sys_msg 【$_Player$】开启了【守护公会枢纽-联合模式】，可通过#L公会城市@@(1000,25400,39200)#n的#L克利斯提尼@@(60000,17700,25500)#n传送到守护枢纽大厅协助作战
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				elseif ret3 = -3
					add_sys_msg_start
					add_sys_msg 同时只能1个公会开启【守护公会枢纽-联合模式】，请结束后再开启
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				else
				endif
			else
				add_sys_msg_start
				add_sys_msg 开启【守护公会枢纽-联合模式】需要支付50000银币
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【守护公会枢纽-联合模式】
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			if yinbi >= 50000
				CLAN_MACHINE_PWORLD_CREATE ret3 1
				if ret3 = -1
					add_sys_msg_start
					add_sys_msg 已开启过【守护公会枢纽-联合模式】，每周最多只能开启1次  
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = -2
					add_sys_msg_start
					add_sys_msg 建立公会的第一周，无法开启【守护公会枢纽-联合模式】
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = 0
					DEC_MONEY 0 50000 20 biaoji
					if biaoji = 0
						ADD_ID_CLAN_MONEY 0 0 20000 0 0
					endif
					
					add_sys_msg_start
					add_sys_msg 【$_Player$】开启了【守护公会枢纽-联合模式】，可通过#L公会城市@@(1000,25400,39200)#n的#L克利斯提尼@@(60000,17700,25500)#n传送到守护枢纽大厅协助作战
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0	
				elseif ret3 = -3
					add_sys_msg_start
					add_sys_msg 同时只能1个公会开启【守护公会枢纽-联合模式】，请结束后再开启
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				else
				endif
			else
				add_sys_msg_start
				add_sys_msg 开启【守护公会枢纽-联合模式】需要支付50000银币
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【守护公会枢纽-联合模式】
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 450
	
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk 【普通模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	add_talk #b#b
	if judian > 0
		add_talk #c186506开启要求：未开启【守护公会枢纽-联合模式】#n
	endif
	add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】
	add_talk 活动时间：每周六1次(06:00-24:00)
	add_talk 会长开启：06:00-22:00
	add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
	add_talk 活动进展：已开启【守护公会枢纽-联合模式】#n
	add_talk #b#b
	add_talk #b#b#c005ebe如果未参加任何模式的【守护公会枢纽】，今天仍可参加【守护公会枢纽-联合模式】#n
	
		option 454 #我已召集会员，开启【守护公会枢纽-普通模式】
		option 1 #返回
	talk
endproc

proc 451
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk 【联合模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	add_talk #c005ebe#b#b4.击杀敌兵获得经验值增加10%
	add_talk #b#b5.任何公会的成员都可参与活动
	add_talk #b#b6.开启的公会可获得20000公会资金#n
	add_talk #b#b
	if judian > 0
		add_talk #c186506开启要求：未开启【守护公会枢纽-普通模式】#n
	endif
	add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】
	add_talk 活动时间：每周六1次(06:00-24:00)
	add_talk 会长开启：06:00-22:00
	add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
	add_talk 活动进展：已开启【守护公会枢纽-普通模式】#n
	add_talk #b#b
	add_talk #b#b#c005ebe如果未参加任何模式的【守护公会枢纽】，今天仍可参加【守护公会枢纽-联合模式】#n
	
		option 455 #我已召集会员，开启【守护公会枢纽-联合模式】
		option 1 #返回
	talk
endproc

proc 452
	strvar shijian zhuangtai moshi judian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk 【普通模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	add_talk #b#b
	if judian > 0
		add_talk #c186506开启要求：未开启【守护公会枢纽-联合模式】#n
	endif
	add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】
	add_talk 活动时间：每周六1次(06:00-24:00)
	add_talk 会长开启：06:00-22:00
	add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
	add_talk 活动进展：已结束#n
	add_talk #b#b
	; if shijian = 1 and zhuangtai = 2
		; add_talk #b#b在每个人的齐心协力下，我们成功以最大的限度守护着枢纽！
	; endif
	add_talk #b#b#c005ebe如果未参加任何模式的【守护公会枢纽】，今天仍可参加【守护公会枢纽-联合模式】#n
	
	option 1 #返回
	talk
endproc

proc 453
	strvar shijian zhuangtai moshi judian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk 【联合模式】规则:
	add_talk #b#b1.敌方袭击一共分为30波
	add_talk #b#b2.公会枢纽被敌方击毁活动失败
	add_talk #b#b3.活动中击杀敌兵，所有人可获得经验
	add_talk #c005ebe#b#b4.击杀敌兵获得经验值增加10%
	add_talk #b#b5.任何公会的成员都可参与活动
	add_talk #b#b6.开启的公会可获得20000公会资金#n
	add_talk #b#b
	if judian > 0
		add_talk #c186506开启要求：未开启【守护公会枢纽-普通模式】#n
	endif
	add_talk #c186506参加要求：未参加任何模式的【守护公会枢纽】
	add_talk 活动时间：每周六1次(06:00-24:00)
	add_talk 会长开启：06:00-22:00
	add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
	add_talk 活动进展：已结束#n
	add_talk #b#b
	; if shijian = 1 and zhuangtai = 2
		; add_talk #b#b在每个人的齐心协力下，我们成功以最大的限度守护着枢纽！
	; endif
	add_talk #b#b#c005ebe如果未参加任何模式的【守护公会枢纽】，今天仍可参加【守护公会枢纽-联合模式】#n
	
	option 1 #返回
	talk
endproc

proc 454
	add_sys_msg_start
	add_sys_msg 公会已开启【守护公会枢纽-联合模式】，今天无法再开启【守护公会枢纽-普通模式】
	set_sys_msg_mode 4 0 0
	msg_info
endproc

proc 455
	add_sys_msg_start
	add_sys_msg 公会已开启【守护公会枢纽-普通模式】，今天无法再开启【守护公会枢纽-联合模式】
	set_sys_msg_mode 4 0 0
	msg_info
endproc
