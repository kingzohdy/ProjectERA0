

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b一群狡猾的窃贼偷走了时间城的宝贵资源“水晶”和“矿石”，夺回资源将会获得战魂徽章作为奖励。
	add_talk #b#b小心，看好你拿到的资源，别被其他玩家抢走了。
	add_talk #b#b#c186506战场提示：完成任意一项获得荣誉、奖章奖励#n
	add_talk #b#b#c1865061、击杀和助攻总数达到25#n
	add_talk #b#b#c1865062、战场积分排名达到前90％#n

	
	task_add_option 3301 1 100
	task_add_option 3302 1 101
	task_add_option 3303 1 102
	
	
	task_add_option 3165 1 103
	task_add_option 3166 1 104
	task_add_option 3167 1 105
	task_add_option 3168 1 106
	
	task_add_option 3100 1 107
	task_add_option 3101 1 108
	task_add_option 3102 1 109
	task_add_option 3103 1 110
	task_add_option 3104 1 111
	task_add_option 3105 1 112
	task_add_option 3106 1 113
	task_add_option 3107 1 114
	
	;strvar Ret 
	;task_stat 3676 1 Ret
	;if Ret = 2 
	;	option 300 这里有张凭证需要你签名
	;endif
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300920 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	option 290 #(默认)参加狂沙角斗场
;	option 293 #(买卖)声望商店
	
	talk
endproc 

proc 293
	OPEN_NPC_SHOP
endproc


PROC 100
	task_do_option 3301 1
ENDPROC
PROC 101
	task_do_option 3302 1
ENDPROC
PROC 102
	task_do_option 3303 1
ENDPROC
PROC 103
	task_do_option 3165 1
ENDPROC
PROC 104
	task_do_option 3166 1
ENDPROC
PROC 105
	task_do_option 3167 1
ENDPROC
PROC 106
	task_do_option 3168 1
ENDPROC


PROC 107
	task_do_option 3100 1
ENDPROC
PROC 108
	task_do_option 3101 1
ENDPROC
PROC 109
	task_do_option 3102 1
ENDPROC
PROC 110
	task_do_option 3103 1
ENDPROC
PROC 111
	task_do_option 3104 1
ENDPROC
PROC 112
	task_do_option 3105 1
ENDPROC
PROC 113
	task_do_option 3106 1
ENDPROC
PROC 114
	task_do_option 3107 1
ENDPROC
proc 290
	OPEN_UI 54
endproc

proc 2
	get_trig_task 1 2 
	if 2 = 3168
		task_fini 3168 1
	endif
endproc 


proc 291

	GET_EVENT_STAT 411 18		
	GET_PLAYER_BASE_ATT 0 40 41 42 	
	if 41 >= 30
	
		IF 411 = 1 
			strvar a
			APPLY_PWORLD_QUEUE 400 a
			if a = 0
				add_sys_msg_start
				add_sys_msg 报名成功！
				set_sys_msg_mode 4 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg 暂时无法报名
				set_sys_msg_mode 4 0 0
				msg_info
			endif
				
		ELSE
			add_sys_msg_start
			add_sys_msg 活动还没开始，无法进入！
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
	

	else
		add_sys_msg_start
		add_sys_msg 角斗场中战斗异常凶险，30级以上玩家才能进入
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
endproc 

;proc 300
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b嗯…确实是公会专用的领取凭证，这就给你签上去。
;;
	;	option 301 #拿出领取凭证让罗宾逊签名

;
;
;	talk
;endproc 

proc 301
	GET_ITEM_NUM_INPAK 5301002 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5301002 1 30
		if 30 != -1
			ADD_ITEM_NUM_INPAK 5301003 1 1 0
		endif
	endif
endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b来时间城这么久了，还不了解它的历史，真是惭愧啊！
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300920 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300920 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300921 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300922 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300923 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300924 1 0 0
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


