
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b倘若你决定了做什么，而且这件事被大家所接受、所喜欢，那么你坚持自己，走下去，一定会得到认可，人们也会帮助你的。

	add_talk #b#b另外为了方便大家，我这里也兼提供环组任务的“我很忙”功能的使用。
		task_add_option 3304 1 100
		task_add_option 3305 1 101

		strvar lv
		GET_PLAYER_BASE_ATT 0 0 lv 1
		
		if lv >= 50
			option 200 我要立刻完成【别针换别墅】
		endif
		
		if lv >= 35 and lv < 45
			option 210 我要立刻完成【环组】阿努比斯神殿
		endif
	
		if lv >= 45 and lv < 55
			option 220 我要立刻完成【环组】十三号矿洞
		endif
	
		if lv >= 55 and lv < 65
			option 230 我要立刻完成【环组】越雾雨林
		endif
		
		if lv >= 65 and lv < 75
			option 240 我要立刻完成【环组】普罗圣地
		endif
		
		if lv >= 75 and lv < 85
			option 250 我要立刻完成【环组】牢城
		endif
	talk
endproc 
proc 100
	task_do_option 3304 1
endproc 
proc 101
	task_do_option 3305 1
endproc 
proc 200
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 50 and lv < 70
		PLAYER_BUSY_UI 14 1
	endif
	if lv >= 70
		PLAYER_BUSY_UI 15 1
	endif
endproc
proc 210
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc 
proc 220
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 
proc 230
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc 
proc 240
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 250
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 75 and lv < 85
		PLAYER_BUSY_UI 20 1
	endif
endproc 