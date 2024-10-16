

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b咩～没见过卖武器的羊吗？！咩～我不是羊！我是卖羊的武器！咩～我是卖武器的羊！咩～都说了我不是羊！我是人一样的羊！你懂了吗？！咩～
	add_talk #b#b#c186506出售各职业的武器#n
	task_add_option 1090 1 102
	task_add_option 1091 1 103

	task_add_option 2022 1 104
	task_add_option 2023 1 105

;环次

	task_stat 4487 1 20
	if 20 = 2
		option 200 #向武器商人雷蒙德赠送一个粽子
	endif
	
	OPTION 101 #(买卖)打开商店
	option 300 #兑换70级【传奇武器】
	option 400 #升档80级【传奇武器】
	
	talk
endproc 


PROC 101
	OPEN_NPC_SHOP
ENDPROC

PROC 102
	task_do_option 1090 1
ENDPROC
PROC 103
	task_do_option 1091 1
ENDPROC
PROC 104
	task_do_option 2022 1
ENDPROC
PROC 105
	task_do_option 2023 1
ENDPROC

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
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
	task_stat 4487 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4487 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4487 1
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


proc 2
	get_trig_task 1 2
	if 2 = 1091
		task_fini 1091 1
	endif 
	if 2 = 2023 
		task_fini 2023 1
	endif 
endproc

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b咩～没见过卖武器的羊吗？！咩～我不是羊！我是卖羊的武器！咩～我是卖武器的羊！咩～都说了我不是羊！我是人一样的羊！你懂了吗？！咩～
	add_talk #b#b#c186506注意：只能兑换属于自己专属职业的传奇装备#n
	option 330 #兑换70级【传奇的主手武器】
	option 340 #兑换70级【传奇的副手武器】
	
	talk
endproc

proc 330
	choose 2 310 301 兑换需要：混合剂20个、重构物质300个\n是否确认兑换？
endproc

proc 310
	strvar lv job gezi num1 num2 ret1 ret2
	GET_PLAYER_BASE_ATT 0 0 lv job
	GET_PACK_SPACE 0 gezi
	
	GET_ITEM_NUM_INPAK 5060302 0 num1
	GET_ITEM_NUM_INPAK 5060304 0 num2
	
	if lv < 70
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换70级【传奇的】主手武器，人物等级需要达到70级
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num1 < 20
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包里不够20个【混合剂】，暂时无法兑换70级【传奇的】主手武器
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num2 < 300
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包里不够300个【重构物质】，暂时无法兑换70级【传奇的】主手武器
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif @gezi < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦，请先整理背包再兑换
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		DEC_ITEM_NUM_INPAK 5060302 20 ret1
		DEC_ITEM_NUM_INPAK 5060304 300 ret2
		if ret1 = 0 and ret2 = 0
			if job = 17
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1016006 1 1 0
			endif
			if job = 18
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1106006 1 1 0
			endif
			if job = 19
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1206006 1 1 0
			endif
			if job = 20
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1516006 1 1 0
			endif
			if job = 33
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1036006 1 1 0
			endif
			if job = 34
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1116006 1 1 0
			endif
			if job = 35
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1256006 1 1 0
			endif
			if job = 36
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1506006 1 1 0
			endif
		endif
	endif
endproc

proc 340
	choose 2 320 301 兑换需要：混合剂20个、重构物质300个\n是否确认兑换？
endproc

proc 320
	strvar lv job gezi num1 num2 ret1 ret2
	GET_PLAYER_BASE_ATT 0 0 lv job
	GET_PACK_SPACE 0 gezi
	
	GET_ITEM_NUM_INPAK 5060302 0 num1
	GET_ITEM_NUM_INPAK 5060304 0 num2
	
	if lv < 70
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换70级【传奇的】副手武器，人物等级需要达到70级
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num1 < 20
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包里不够20个【混合剂】，暂时无法兑换70级【传奇的】副手武器
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num2 < 300
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包里不够300个【重构物质】，暂时无法兑换70级【传奇的】副手武器
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif @gezi < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦，请先整理背包再兑换
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		DEC_ITEM_NUM_INPAK 5060302 20 ret1
		DEC_ITEM_NUM_INPAK 5060304 300 ret2
		if ret1 = 0 and ret2 = 0
			if job = 17
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1066006 1 1 0
			endif
			if job = 18
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1186006 1 1 0
			endif
			if job = 19
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1286006 1 1 0
			endif
			if job = 20
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1586006 1 1 0
			endif
			if job = 33
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1046006 1 1 0
				call 601
			endif
			if job = 34
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1186016 1 1 0
			endif
			if job = 35
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1266006 1 1 0
			endif
			if job = 36
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1586016 1 1 0
			endif
		endif
	endif
endproc

proc 400
	OPEN_UI 92
endproc
