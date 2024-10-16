
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b您好，基尔特商会最新服务为您提供便利，忙得没时间做活动的朋友，现在可以雇佣替身机器人来帮你完成。
	add_talk #b#b完成后可以获得活动对应的奖励。
	add_talk #b#b
	add_talk #b#b暂时只能帮您完成别针换别墅、公会事务、环组和赏善罚恶四种任务。
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 50
		option 100 我要立刻完成【别针换别墅】
	endif
		
	if lv >= 35 and lv < 45
		option 110 我要立刻完成【环组】阿努比斯神殿
	endif
	
	if lv >= 45 and lv < 55
		option 120 我要立刻完成【环组】十三号矿洞
	endif
	
	if lv >= 55 and lv < 65
		option 130 我要立刻完成【环组】越雾雨林
	endif
	
	if lv >= 65 and lv < 75
		option 140 我要立刻完成【环组】普罗圣地
	endif
	
	if lv >= 75 and lv < 85
		option 150 我要立刻完成【环组】牢城
	endif
	
	if lv > 40
		option 160 我要立刻完成一次【赏善罚恶】
	endif
	
	strvar Ret1 Pos
	player_clan_pos_info Ret1 0 0 0 Pos
	
	if Ret1 = 0 and Pos != 0
		if lv > 40
			option 170 我要立刻完成【公会事务】
		endif
	endif
	
	talk
endproc 

proc 100
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 50 and lv < 70
		PLAYER_BUSY_UI 14 1
	endif
	if lv >= 70 and lv <= 120
		PLAYER_BUSY_UI 15 1
	endif
endproc
proc 110
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc 
proc 120
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 
proc 130
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc 
proc 140
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 150
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 75 and lv < 85
		PLAYER_BUSY_UI 20 1
	endif
endproc 
proc 160
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 40 and lv < 45
		PLAYER_BUSY_UI 6 1
	endif
	if lv >= 45 and lv < 50
		PLAYER_BUSY_UI 7 1
	endif
	if lv >= 50 and lv < 55
		PLAYER_BUSY_UI 8 1
	endif
	if lv >= 55 and lv < 60
		PLAYER_BUSY_UI 9 1
	endif
	if lv >= 60 and lv < 65
		PLAYER_BUSY_UI 10 1
	endif
	if lv >= 65 and lv < 70
		PLAYER_BUSY_UI 11 1
	endif
	if lv >= 70 and lv < 75
		PLAYER_BUSY_UI 12 1
	endif
	if lv >= 75 and lv <= 120
		PLAYER_BUSY_UI 13 1
	endif
endproc
proc 170
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv > 40 and lv <= 50
		PLAYER_BUSY_UI 24 1
	endif
	if lv > 50 and lv <= 60
		PLAYER_BUSY_UI 25 1
	endif
	if lv > 60 and lv <= 70
		PLAYER_BUSY_UI 26 1
	endif
	if lv > 70 and lv <= 80
		PLAYER_BUSY_UI 27 1
	endif
endproc
   

