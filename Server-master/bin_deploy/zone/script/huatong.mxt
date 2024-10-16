
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你好啊，买束鲜花送给心爱的人吧!
	
	;五一活动
	task_stat 4437 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	strvar Ret num1 num2
	GET_ITEM_NUM_INPAK 5300761 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	
	GET_ITEM_NUM_INPAK 5300911 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	
	strvar RetTsk
	task_stat 3125 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	
	option 800 #想要买一块“浓情巧克力”吗？
	option 900 #背饰幻化
	
	talk
endproc

proc 900
	OPEN_UI 93
endproc

proc 800
	dialog 1
	add_talk $_npc$:
	add_talk #b#b爱情就像浓郁巧克力，苦中带甜却让人心馋。
	add_talk #b#b#cFF0080向心上人送上巧克力，表达心中执着的爱意。#n
	add_talk #b#b
	add_talk #b#b#c005ebe成功表白后双方可成为【情侣】#n
	add_talk #b#b售价：#c18650610000银币#n
	
	option 801 #我要买一块“浓情巧克力”
	
	talk
endproc

proc 801
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		dec_money 0 10000 20 mret
		if mret = -1
			add_sys_msg_start
			add_sys_msg 你身上携带的银币不够，暂时无法购买巧克力
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			; 银币不足会打开快捷购买方式
			return
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251131 1 1 0
			add_sys_msg_start
			add_sys_msg 你从谢莉手上小心翼翼的接过了“浓情巧克力”
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包已装不下了，请留出1格背包格子后再购买
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc


proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b谢谢你，圣诞老公公！这份礼物我一定会好好爱惜的！
		option 210 #交出圣诞礼物
	talk
endproc 
proc 210
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
			task_fini 3125 1
		endif
	endif
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b别针换别墅，虽然听起来有点荒诞，不过有志者事竟成，也许你会成功呢！
	add_talk #b#b哎，别墅我是没有，只有一本零纪元史记，还是克尔泽先生送给我的呢，我就用这个跟你交换吧。

		option 501 #交换

	talk
	
endproc
proc 501
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300761 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300761 1 1
			
	;		rand Rad 100
	;		if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300762 1 0 0
	;		endif
	;		if @Rad >= 80 and @Rad < 95
	;			ADD_ITEM_NUM_INPAK 5300763 1 0 0
	;		endif
	;		if @Rad >= 95 and @Rad < 99
	;			ADD_ITEM_NUM_INPAK 5300764 1 0 0
	;		endif
	;		if @Rad = 99
	;			ADD_ITEM_NUM_INPAK 5300765 1 0 0
	;		endif
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当有一天唐纳成了大富翁，有了别墅之后，这枚别针就会变得非常有纪念价值，嗯，我和你换吧，虽然我没有别墅，但是我有能帮你打开成功大门的钥匙！
	add_talk #b#b最近世界上掀起了一阵收集零纪元史的风潮，我就吃点亏，用零纪元史与你换，如何？
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300911 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300911 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300912 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300913 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300914 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300915 1 0 0
		;	endif
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

;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4437 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，大#Gender1#，我长大了也要向艾劳姐姐那样漂亮能干！
			option 511 #我看好你哟！
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“慰问物资”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
