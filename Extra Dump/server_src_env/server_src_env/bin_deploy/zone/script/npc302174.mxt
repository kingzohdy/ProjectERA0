;npc 吉米

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b万里晴空，壮丽山河，勇士如林，看天地方圆，普天同庆江河歌唱，遍地流芳，世界稳定，人民幸福，自由联盟谱新章……
	add_talk #b#b
	add_talk #b#b在此国庆佳节之际，我仅代表时间城议会向各位勇士奉上最诚挚的祝福与犒赏！
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		option 200 领取在线大礼
	elseif @13 = 0 and @12 < 40 
		GET_PLAYER_DAILY_FLAG 0 15 22
		if @22 = 0
			option 101 领取见面礼
		endif
	else
	endif
		
	talk
endproc 

proc 101
	GET_PLAYER_DAILY_FLAG 0 15 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法领取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 15 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5012117 5 1 14 
		add_sys_msg_start
		add_sys_msg 国庆节快乐！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 200
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
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b 每天早上6点重置在线时间，当天在线时间可累计。
	add_talk #b#b
	add_talk #b#b您今天的在线时间为：$@10$时$@11$分
	
	GET_PLAYER_DAILY_FLAG 0 10 20
	GET_PLAYER_DAILY_FLAG 0 11 21
	GET_PLAYER_DAILY_FLAG 0 13 22
	
	if @20 = 0
		option 201 领取见面礼
	endif
	
	if @21 = 0
		option 202 领取在线满2小时礼物
	endif
	
	if @22 = 0
		option 203 领取在线满5小时礼物
	endif
		
	talk
endproc

proc 201
	GET_PLAYER_DAILY_FLAG 0 10 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 < 2
		add_sys_msg_start
		add_sys_msg 吉米为你准备了两种礼物哟，请整理背包预留2个空格子再来领取吧！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 10 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
		ADD_BIND_ITEM_NUM_INPAK 5060050 10 1 15 
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 202
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 11 19
	;0表示未设置
	if 19 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		add_sys_msg_start
		add_sys_msg 抱歉，您今天在线时间不足2小时，请稍后再来领取。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 < 2
			add_sys_msg_start
			add_sys_msg 吉米为你准备了两种礼物哟，请整理背包预留2个空格子再来领取吧！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 11 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
			ADD_BIND_ITEM_NUM_INPAK 5060050 30 1 15 
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 203
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 13 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 18000
		add_sys_msg_start
		add_sys_msg 抱歉，您今天在线时间不足5小时，请稍后再来领取。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 < 2
			add_sys_msg_start
			add_sys_msg 吉米为你准备了两种礼物哟，请整理背包预留2个空格子再来领取吧！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 13 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5060047 10 1 14 
			ADD_BIND_ITEM_NUM_INPAK 5060050 60 1 15
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
