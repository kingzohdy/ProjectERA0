proc 1
	dialog 1
	add_talk $_npc$:
	strvar yue ri
	GET_NOW_TIME yue ri 0 0 0 0 0
	
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		add_talk #b#b谁都曾有过需要帮助的时候，你们的一点小善意，对我来说就是莫大的恩惠。
		add_talk #b#b平日多得了大家的帮助与信任，如今小的有一份心意想回赠给大家，希望大家能有个温暖的感恩节。
	else
		add_talk #b#b这位朋友，看你骨骼精奇，器宇非凡，天生就是练武的好材料，维护世界和平就靠你了。等等，先别走啊，赏几个小钱花花吧…
		add_talk #b#b哎！别急！作为一个高素质流浪汉！不会白收你钱的，嘿嘿……
		add_talk #b#b偷偷告诉你，我这可是有不少神奇锦囊!嘿嘿……你懂的
	endif
		add_talk #b#b
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
		
	if jnum > 5
		sub jnum 5
		add_talk #c0040FF今日捐献次数：5/5 #n
		add_talk #c0040FF翡翠徽章特权捐献次数：$@jnum$/3 #n
	else
	
		add_talk #c0040FF今日捐献次数：$@jnum$/5 #n
		add_talk #c0040FF翡翠徽章特权额外捐献次数：0/3 #n
	
	endif
	strvar time ret
	get_role_var 27 32 1 time 
	get_role_var 31 8 1 10
	is_same_day @time ret
	if ret = -1
		var 10 0
		set_role_var 31 8 @10 1
	endif
	if 10 = 0
		add_talk #c0040FF水晶徽章特权免费次数：0/1 #n
	else
		add_talk #c0040FF水晶徽章特权免费次数：1/1 #n
	endif
	
;	TASK_STAT 3115 1 20
	GET_PLAYER_TASK_SPRITE_FLAG 1 20
	if 20 > 0
	;  	task_add_option 3115 1 99
	endif
	
	;五一活动
	task_stat 4436 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	task_add_option 3116 1 98
	task_add_option 3117 1 97
	task_add_option 3148 1 96
	
	strvar RetTsk
	task_stat 3126 1 RetTsk
	
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		option 300 流浪汉的【感恩节】大回赠！！
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300916 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	
;	option 100 施舍100银币
;	option 300 施舍1000银币
;	option 500 施舍3000银币

	;task_add_option 3110 1 99
	
	strvar retm
	GOLD_RATE_GET_MONEY retm
	if retm != -1
		if retm >= 0 and retm <= 4998
			option 801 捐献4998银币
		else
			option 800 捐献$@retm$银币
		endif
	endif
	
	option 822 #我有水晶徽章特权，我来抢1个
	
	talk

endproc 
proc 800
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
	GET_STAR_INFO 2 20
	if 20 = 1
	
		if jnum >= 8
			add_sys_msg_start
			add_sys_msg 流浪汉的锦囊已所剩无几，只好推辞了你的捐献
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	else
		if jnum >= 5
			add_sys_msg_start
			add_sys_msg 流浪汉的锦囊已所剩无几，只好推辞了你的捐献
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		GOLD_RATE_DEC_MONEY mret 
		if mret = -1
			add_sys_msg_start
			add_sys_msg 由于流浪汉正忙着数钱，似乎没听到你的搭理
			set_sys_msg_mode 3 0 0
			msg_info
		 
			return
		elseif mret = -2
			add_sys_msg_start
			add_sys_msg 下次试试多带点银币，说不定意外的收获在等着你
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			;银币不足会打开快捷购买方式
			
			return
		else
			GET_ID_PLAYER_INFO 0 8 30
			rand 40 100
			if 40 = 0
			
				
				add_sys_msg_start
				if 30 < 60
					ADD_ITEM_NUM_INPAK 5200054 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶浓缩转移石”，似乎别人都没有呢
				endif
				if 30 >= 60 and 30 < 70
					ADD_ITEM_NUM_INPAK 5200055 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶金属转移石”，似乎别人都没有呢
				endif
				if 30 >= 70 and 30 < 80
					ADD_ITEM_NUM_INPAK 5200056 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶工艺转移石”，似乎别人都没有呢
				endif
				if 30 >= 80
					ADD_ITEM_NUM_INPAK 5200057 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶化学转移石”，似乎别人都没有呢
				endif
				set_sys_msg_mode 3 0 0
				msg_info
				
			else
				ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
				add_sys_msg_start
				add_sys_msg 流浪汉小心翼翼的掏出一个“神奇锦囊”塞到你手中
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
			GET_EVENT_STAT 36 99
			if @36 = 1
			;中秋活动正在进行中
				ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
				if @37 = 0
					strvar npcmid
					GET_MAP_NPC_ID  304013 npcmid 1
					MON_SPEECH  @npcmid 0 0 1 值此中秋佳节之际，这两盒月饼就当作是回赠您的谢礼，多谢您这么长时间的帮助。
				endif
			endif
			
			call 811
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包已放不下锦囊，流浪汉只好推辞了你的捐献
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc 

proc 801
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
	GET_STAR_INFO 2 20
	if 20 = 1
	
		if jnum >= 8
			add_sys_msg_start
			add_sys_msg 流浪汉的锦囊已所剩无几，只好推辞了你的捐献
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	else
		if jnum >= 5
			add_sys_msg_start
			add_sys_msg 流浪汉的锦囊已所剩无几，只好推辞了你的捐献
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		; GOLD_RATE_DEC_MONEY mret
		dec_money 0 4998 20 mret
		if mret = -1
			add_sys_msg_start
			add_sys_msg 下次试试多带点银币，说不定意外的收获在等着你
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			; 银币不足会打开快捷购买方式
			
			return
		
		; if mret = -1
			; add_sys_msg_start
			; add_sys_msg 由于流浪汉正忙着数钱，似乎没听到你的搭理
			; set_sys_msg_mode 3 0 0
			; msg_info
		 
			; return
		; elseif mret = -2
			; add_sys_msg_start
			; add_sys_msg 下次试试多带点银币，说不定意外的收获在等着你
			; set_sys_msg_mode 3 0 0
			; msg_info
			
			; BUY_HETONG 1
			;银币不足会打开快捷购买方式
			
			; return
		else
		GOLD_RATE_DAYNUM_ADD 1
			if 1 = 0
				GET_ID_PLAYER_INFO 0 8 30
				rand 40 100
				if 40 = 0
				
					
					add_sys_msg_start
					if 30 < 60
						ADD_ITEM_NUM_INPAK 5200054 1 0 1
						add_sys_msg 流浪汉悄悄塞给你一个“高阶浓缩转移石”，似乎别人都没有呢
					endif
					if 30 >= 60 and 30 < 70
						ADD_ITEM_NUM_INPAK 5200055 1 0 1
						add_sys_msg 流浪汉悄悄塞给你一个“高阶金属转移石”，似乎别人都没有呢
					endif
					if 30 >= 70 and 30 < 80
						ADD_ITEM_NUM_INPAK 5200056 1 0 1
						add_sys_msg 流浪汉悄悄塞给你一个“高阶工艺转移石”，似乎别人都没有呢
					endif
					if 30 >= 80
						ADD_ITEM_NUM_INPAK 5200057 1 0 1
						add_sys_msg 流浪汉悄悄塞给你一个“高阶化学转移石”，似乎别人都没有呢
					endif
					set_sys_msg_mode 3 0 0
					msg_info
				
				else
					ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
					add_sys_msg_start
					add_sys_msg 流浪汉小心翼翼的掏出一个“神奇锦囊”塞到你手中
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			
				GET_EVENT_STAT 36 99
				if @36 = 1
				;中秋活动正在进行中
					ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
					if @37 = 0
						strvar npcmid
						GET_MAP_NPC_ID  304013 npcmid 1
						MON_SPEECH  @npcmid 0 0 1 值此中秋佳节之际，这两盒月饼就当作是回赠您的谢礼，多谢您这么长时间的帮助。
					endif
				endif
			call 811
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包已放不下锦囊，流浪汉只好推辞了你的捐献
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc

proc 822 

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法领取道具
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif


	GET_STAR_INFO 1 20
	
	if 20 = 1
		get_role_var 31 8 1 10
		if 10 < 1
			GET_ID_PLAYER_INFO 0 8 30
			rand 40 100
			if 40 = 0
				add_sys_msg_start
				if 30 < 60
					ADD_ITEM_NUM_INPAK 5200054 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶浓缩转移石”，似乎别人都没有呢
				endif
				if 30 >= 60 and 30 < 70
					ADD_ITEM_NUM_INPAK 5200055 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶金属转移石”，似乎别人都没有呢
				endif
				if 30 >= 70 and 30 < 80
					ADD_ITEM_NUM_INPAK 5200056 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶工艺转移石”，似乎别人都没有呢
				endif
				if 30 >= 80
					ADD_ITEM_NUM_INPAK 5200057 1 0 1
					add_sys_msg 流浪汉悄悄塞给你一个“高阶化学转移石”，似乎别人都没有呢
				endif
				
				set_sys_msg_mode 3 0 0
				msg_info
			else
				ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
				add_sys_msg_start
				add_sys_msg 流浪汉小心翼翼的掏出一个“神奇锦囊”塞到你手中
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
			GET_EVENT_STAT 36 99
			if @36 = 1
			;中秋活动正在进行中
				ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
				if @37 = 0
					strvar npcmid
					GET_MAP_NPC_ID  304013 npcmid 1
					MON_SPEECH  @npcmid 0 0 1 值此中秋佳节之际，这两盒月饼就当作是回赠您的谢礼，多谢您这么长时间的帮助。
				endif
			endif
			
			call 811
			GET_TIME_SEC time
			set_role_var 27 32 @time 1			
			add 10 1
			set_role_var 31 8 @10 1	
			
		else
			add_sys_msg_start
			add_sys_msg 还要抢几次？看我老人家好欺负？
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 知道怎么变成高富帅吗？按个O键试试……
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这突如其来的礼物，如同那寒冬里的一根火柴，温暖着我那破碎的心，谢谢你，$_player$，我会谨记于心的！等我发了财一定不忘你！
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
			task_fini 3126 1
		endif
	endif
endproc 


proc 100
	strvar Lock
        pkg_is_unlock 1 Lock
        if Lock = 0
		
                return
        endif
	strvar m
	var m 100
	DEC_MONEY_TEST 0 @m 20 1
	 if @1 = -1
                ADD_SYS_MSG_START
                TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			 ADD_SYS_MSG 下次试试多带点银币，说不定有意外的收获。
		else
			 ADD_SYS_MSG 身上的银币不足够施舍。
		endif
                SET_SYS_MSG_MODE 1 0 0
                MSG_INFO
				
			BUY_HETONG 1
			;银币不足会打开快捷购买方式
			
		return
        endif
	
	DEC_MONEY 0 @m 20 1
	
	;通用事件数
	strvar num
	var num 21
	;随机数以10000为基数
	strvar r
	;各事件概率31开始
	;70级以下概率
	var 31 1000 

;	var 32 4000 
;	var 33 1000 
;	var 34 100 

	var 32 0 
	var 33 0 
	var 34 0 

	var 35 0
	var 36 0
	var 37 0

	var 38 400
	var 39 400
	var 40 400
	var 41 400
	var 42 400

	;各事件proc 号61开始
	var 61 101 
	var 62 102 
	var 63 103 
	var 64 104
	var 65 105 
	var 66 106 
	var 67 107

	var 68 108
	var 69 109
	var 70 110
	var 71 111
	var 72 112

	strvar Ret Sex Level Job
        get_player_base_att  Ret Sex Level Job
	
	;70级后概率
;	if @level >= 70
;		var 32 0 
;		var 33 0 
;		var 34 0 
;		;没接任务
;		TASK_STAT 3115 1 5
;		if 1 = 0 and 5 = 0
;
;			var 35 4000 
;			var 36 1000 
;			var 37 5
;		endif
;	endif

	strvar stat



	rand r 10000

		if r < @31
		call @61
		return
	endif
	sub r @31
	if r < @32
		call @62
		return
	endif
	sub r @32
	if r < @33
		call @63
		return
	endif
	sub r @33
	if r < @34
		call @64
		return
	endif
	sub r @34
	if r < @35
		call @65
		return
	endif
	sub r @35
	if r < @36
		call @66
		return
	endif
	sub r @36
	if r < @37
		call @67
		return
	endif
	sub r @37

		if r < @38
		call @68
		return
	endif
	sub r @38
	if r < @39
		call @69
		return
	endif
	sub r @39
	if r < @40
		call @70
		return
	endif
	sub r @40
	if r < @41
		call @71
		return
	endif
	sub r @41
	if r < @42
		call @72
		return
	endif
	sub r @42

	;proc 号
	strvar p
	var p 20
	mod r @num
	add p @r

;	dialog 1
	add_talk $_npc$:
;	add_talk #b#b100…$@r$.....$@p$..
;	talk
	;return
	if P > 40
		var p 20
	endif

	call @p

endproc

proc 108
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这是我无以回报的答谢。


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 1 1800 1
	
endproc

proc 109
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这是我无以回报的答谢。


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 2 1800 1
	
endproc

proc 110
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这是我无以回报的答谢。


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 3 1800 1
	
endproc

proc 111
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这是我无以回报的答谢。


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 4 1800 1
	
endproc

proc 112
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这是我无以回报的答谢。


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 5 1800 1
	
endproc

; proc 300
	; strvar Lock
        ; pkg_is_unlock 1 Lock
        ; if Lock = 0
		
                ; return
        ; endif
	; strvar m
	; var m 1000
	; DEC_MONEY_TEST 0 @m 20 1
	 ; if @1 = -1
                ; ADD_SYS_MSG_START
                ; TASK_STAT 3115 1 5
		; if 1 = 0 and 5 = 0
			 ; ADD_SYS_MSG 下次试试多带点银币，说不定有意外的收获。
		; else
			 ; ADD_SYS_MSG 身上的银币不足够施舍。
		; endif
                ; SET_SYS_MSG_MODE 1 0 0
                ; MSG_INFO
			
				; BUY_HETONG 1
				;银币不足会打开快捷购买方式	
				
		; return
		
			
        ; endif

	; DEC_MONEY 0 @m 20 1

	;通用事件数
	; strvar num
	; var num 21
	;随机数以10000为基数
	; strvar r
	;各事件概率31开始
	;70级以下概率
	; var 401 1000
	; var 402 500
	; var 403 500
	; var 404 500
	; var 405 500

					;	var 406 1000
					;	var 407 4000
					;	var 408 300
	
	; var 406 0
	; var 407 0
	; var 408 0

	; var 409 0
	; var 410 0
	; var 411 0
	
	;各事件proc 号701开始
	; var 701 301 
	; var 702 302 
	; var 703 303 
	; var 704 304 
	; var 705 305 
	; var 706 306 
	; var 707 307 
	; var 708 308 
	; var 709 309 
	; var 710 310 
	; var 711 311 

	; strvar Ret Sex Level Job
        ; get_player_base_att  Ret Sex Level Job
	
					;70级后概率,事件号
					;	if @level >= 70
					;		
					;
					;			var 406 0
					;			var 407 0
					;			var 408 0
					;		;没接任务
					;		TASK_STAT 3115 1 5
					;		if 1 = 0 and 5 = 0
					;			var 409 1000
					;			var 410 4000
					;			var 411 53
					;		endif
					;	endif

	; rand r 10000

		; if r < @401
		; call @701
		; return
	; endif
	; sub r @401

	; if r < @402
		; call @702
		; return
	; endif
	; sub r @402

	; if r < @403
		; call @703
		; return
	; endif
	; sub r @403

	; if r < @404
		; call @704
		; return
	; endif
	; sub r @404

	; if r < @405
		; call @705
		; return
	; endif
	; sub r @405

	; if r < @406
		; call @706
		; return
	; endif
	; sub r @406

	; if r < @407
		; call @707
		; return
	; endif
	; sub r @407

	; if r < @408
		; call @708
		; return
	; endif
	; sub r @408

	; if r < @409
		; call @709
		; return
	; endif
	; sub r @409

	; if r < @410
		; call @710
		; return
	; endif
	; sub r @410

	; if r < @411
		; call @711
		; return
	; endif
	; sub r @411

	
	;proc 号
	; strvar p
	; var p 20
	; mod r @num
	; add p @r

	; if P > 40
		; var p 20
	; endif

	; call @p
; endproc

proc 500
	strvar Lock
        pkg_is_unlock 1 Lock
        if Lock = 0
		
                return
        endif

	strvar m
	var m 3000

	DEC_MONEY_TEST 0 @m 20 1
	 if @1 = -1
                ADD_SYS_MSG_START
		TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			 ADD_SYS_MSG 下次试试多带点银币，说不定有意外的收获。
		else
			 ADD_SYS_MSG 身上的银币不足够施舍。
		endif
               
                SET_SYS_MSG_MODE 1 0 0
                MSG_INFO
				
				BUY_HETONG 1
				;银币不足会打开快捷购买方式
				
		return
        endif

	DEC_MONEY 0 @m 20 1

	;通用事件数
	strvar num
	var num 21
	;随机数以10000为基数
	strvar r
	;各事件概率401开始
	;70级以下概率
	var 401 1
	var 402 1000
;	var 403 4000
;	var 404 1000
;	var 405 500

	var 403 0
	var 404 0
	var 405 0

	var 406 0
	var 407 0
	var 408 0

	;已经称号善行者
	var 1 0
	GET_PLAYER_ID 1 id
	HAS_ID_PLAYER_DESIGNATION @id 123 1
	if @1 = 1
		var 401 0
	endif
	
	;各事件proc 号701开始
	var 701 501 
	var 702 502 
	var 703 503 
	var 704 504 
	var 705 505 
	var 706 506 
	var 707 507 
	var 708 508 

	strvar Ret Sex Level Job
        get_player_base_att  Ret Sex Level Job
	
	;70级后概率,事件号
	if @level >= 70

		var 403 0
		var 404 0
		var 405 0

		;没接任务
		TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			var 406 4000
			var 407 1000
			var 408 160
		endif
	endif

	rand r 10000

	if r < @401
		call @701
		return
	endif
	sub r @401

	if r < @402
		call @702
		return
	endif
	sub r @402

	if r < @403
		call @703
		return
	endif
	sub r @403

	if r < @404
		call @704
		return
	endif
	sub r @404

	if r < @405
		call @705
		return
	endif
	sub r @405

	if r < @406
		call @706
		return
	endif
	sub r @406

	if r < @407
		call @707
		return
	endif
	sub r @407

	if r < @408
		call @708
		return
	endif
	sub r @408

	

	
	;proc 号
	strvar p
	var p 20
	mod r @num
	add p @r

	if P > 40
		var p 20
	endif

	call @p
endproc



;通用事件
proc 20
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，右侧快捷可以自定义自己需要的道具，技能，方便战斗时候使用。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 21
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，在小地图上点击相应的位置，人物可以自动到达相应的地方。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 22
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，人物到达三十级时，可以前往时间城的“公会管理员”处，创建自己的公会。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 23
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，您可以在时间城的“公会管理员”处，通过“公会查询”功能，选择要加入的公会。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 24
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，您可以在“游戏设置”的“信息设置”中设置个人偏好的头顶显示信息。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 25
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，您可以在“游戏设置”的“按键设置”中设置个人偏好的快捷键。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 26
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，按F11键可以把玩家和宠物的名字隐藏起来！
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 27
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，按F12键可以把摊位、宠物和队友以外的玩家外形隐藏起来！
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 28
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，按TAB键会自动选中周围的怪！
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 29
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，如果任务途中遇到问题导致无法完成，可以放弃任务重新领取。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 30
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，好像有人从龙蛋谷那头霸王龙身上找到霸王龙的宠物蛋。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 31
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，好像有人从盘山道的野人酋长身上找到了野人酋长的宠物蛋。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 32
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，狂沙角斗场奖励的生存奖章和荣誉点数，似乎能向时间城的梅杰或巴菲换取不错的装备。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 33
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，欢乐嘉年华的箱子中能开出“骑士之魂”坐骑。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 34
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，绿洲风暴战场奖励的战魂徽章和荣誉点数，似乎能向时间城的梅杰或巴菲换取不错的装备。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 35
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，按\键可以把界面隐藏起来！
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 36
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，好像有人从黄金矿井那部矿井收割机上找到守雾者的宠物蛋。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 37
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，学会一项科技生产专业，你可以制造很多买不到的东西。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 38
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，商会的护送任务可以获得大量的金钱。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是……呵呵

	talk
endproc

proc 39
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，找时间城的莉莉可以传送到彩虹岛捕捉守雾者、野人酋长、霸王龙。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是…呵呵

	talk
endproc

proc 40
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	add_talk #b#b据我所知，大地之心可以获得让人起死回生的重生契约。
	add_talk #b#b……
	add_talk #b#b哎！别急！我知道的小秘密还多着呢！只是…呵呵

	talk
endproc

;100银币事件
proc 101
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我都没见过这么吝啬的人，买根牙签都不够啊…
	talk
	strvar id
	GET_PLAYER_ID 1 id
	;小气鬼30分钟
	target_add_status 1 @id 3005 1 1800 1
endproc

proc 102
	dialog 1
	add_talk $_npc$:
		add_talk #b#b…看在这点钱的份上，我就悄悄告诉你好了…
	add_talk #b#b在这世界上仍存在着各种精灵，其中最广为流传的是五元素精灵，它们分别是：
	add_talk #b#b雷神——电元素
	add_talk #b#b山神——土元素
	add_talk #b#b风神——风元素
	add_talk #b#b影子——暗元素
	add_talk #b#b天使——光元素
	add_talk #b#b当世界再次陷入混乱时，这些精灵将开始挑选属于它们的勇者，从归于此！拯救世界！
	add_talk #b#b暂时就只能说到这了，欲知下文，呵呵…你懂的。

	talk
endproc

proc 103
	dialog 1
	add_talk $_npc$:
	add_talk #b#b传说中的风神象征着暴风！是唯一没有实体的精灵，只要是所到之处都将被它夷为平川！
	add_talk #b#b然而当风神被封印后，这个封印着它的魂器便失落在这世界上。有人说藏在了某个神殿中，有人说遗落在某个无人踏迹的地方，甚至有人说经过长年的岁月已经被摧毁了。
	add_talk #b#b但我说，这个封印着风神的魂器仍存在世界上，如果你还想知道更多，或许可以多来找找我，呵呵…你懂的。

	talk
endproc

proc 104
	dialog 1
	add_talk $_npc$:
		add_talk #b#b我曾发誓，哪天遇见那位命运之子，若他助我实现愿望，就把家族世代相传的秘密匣子送给他。
	add_talk #b#b虽然你不失为旷世奇葩，但你现在的实力还不足以助我实现愿望， 70级后再来找我吧，若你就是命运安排的那个人，我会把一切都告诉你。

	talk
endproc

proc 105
	dialog 1
	add_talk $_npc$:
		add_talk #b#b…钱是不多…但我既然收得你钱就肯定会守信。
	add_talk #b#b在这个混乱的世代，沉睡已久的精灵早已开始蠢蠢欲动，四处挑选属于他们的勇者，而这些被挑选上的人，我们称之为命运之子。
	add_talk #b#b其中封印精灵魂器的下落各有说法，最争论不休的应该就是风神的魂器了，对于这点，我个人也有一点小小的结论。
	add_talk #b#b至于结论如何…我想这样珍贵的情报，你是否应该再付出点努力等价交换，呵呵…你懂的。
	talk
endproc

proc 106
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每个人都以为封印着风神的魂器早已失落在这世界上，可是一直有个秘密我没有告诉任何人。
	add_talk #b#b我的家族世代相传着一个秘密匣子，经过多年的调查，与家族流传下来的信息，我能肯定这个匣子就是装载着风神的魂器。
	add_talk #b#b虽然现在我已经一贫如洗，但匣子从未离开身边半步，如果你想了解更多匣子的事情，请把它当作我们之间的小秘密。另外…天上可是没有掉下来的馅饼呢，呵呵…你懂的。

	talk
endproc

proc 107
	dialog 1
	add_talk $_npc$:
	
	add_talk #b#b见你如此有诚意，又迫切着拯救世界的份上，作为新世纪有素质修养的流浪汉，我就把你当作命运之子吧。
	add_talk #b#b不过你还是得先帮我个忙，我的家族相传下来的神秘匣子，很有可能封印着风神，所以你还得帮我实现愿望，顺带证明你是足够拥有风神的力量。

	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	;task_add_option 1001 1 99

	talk

	
endproc

proc 96
	task_do_option 3148 1
endproc

proc 97
	task_do_option 3117 1
endproc

proc 98
	task_do_option 3116 1
endproc

proc 99
	task_do_option 3115 1
endproc
;300银币

; proc 301
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b愿上帝保佑你，感谢你的无私，好心人。
	; GET_PLAYER_ID 1 id
	;好人光辉30分钟
	; target_add_status 1 @id 3005 2 1800 1
	; talk
; endproc

; proc 302
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b请允许我献上崇高的祝福。
	; GET_PLAYER_ID 1 id
	; 好人移动10分钟
	; target_add_status 1 @id 3148 1 1800 1
	; talk
; endproc

; proc 303
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b请允许我献上崇高的祝福。
	; GET_PLAYER_ID 1 id
	;好人命中30分钟
	; target_add_status 1 @id 3148 2 1800 1
	; talk
; endproc

; proc 304
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b请允许我献上崇高的祝福。
	; GET_PLAYER_ID 1 id
	;好人闪避30分钟
	; target_add_status 1 @id 3148 3 1800 1
	; talk
; endproc

proc 305
	dialog 1
	add_talk $_npc$:
	add_talk #b#b请允许我献上崇高的祝福。
	GET_PLAYER_ID 1 id
	;好人暴击30分钟
	target_add_status 1 @id 3148 4 1800 1
	talk
endproc

proc 306
	dialog 1
	add_talk $_npc$:
		add_talk #b#b曾经我也家财万贯，直到一次次命运的捉弄，扒光了我所有财产，除了剩下的一个秘密匣子。它是我家族里代代相传下来的，既然至今它还留在我手上，或许是一种启示，冥冥中注定我将遇上某些转折。
	add_talk #b#b至于这个匣子的秘密，留下了这样一句话“命运之日，狂风将至”，似乎在传达匣子里装着什么不得了的东西。至于剩下的消息，我暂时只能说到这了，如果你还想了解些什么…呵呵…你懂的。

	talk
endproc

proc 307
	dialog 1
	add_talk $_npc$:
		add_talk #b#b传说中的风神象征着暴风！是唯一没有实体的精灵，只要是所到之处都将被它夷为平川！
	add_talk #b#b然而当风神被封印后，这个封印着它的魂器便失落在这世界上。有人说藏在了某个神殿中，有人说遗落在某个无人踏迹的地方，甚至有人说经过长年的岁月已经被摧毁了。
	add_talk #b#b但我说，这个封印着风神的魂器仍存在世界上，如果你还想知道更多，或许可以多来找找我，呵呵…你懂的。

	talk
endproc

proc 308
	dialog 1
	add_talk $_npc$:
		add_talk #b#b我曾发誓，哪天遇见那位命运之子，若他助我实现愿望，就把家族世代相传的秘密匣子送给他。
	add_talk #b#b虽然你不失为旷世奇葩，但你现在的实力还不足以助我实现愿望， 70级后再来找我吧，若你就是命运安排的那个人，我会把一切都告诉你。

	talk
endproc

proc 309
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我曾发誓，哪天遇见那位命运之子，若他助我实现愿望，就把家族世代相传的秘密匣子送给他。
	add_talk #b#b我相信这个匣子仍留在我的身边，或许是一次命运的启示，反正我留着也没用。再怎么说我也是新世纪有素质修养的模范流浪汉，不过是不是命运安排就要看他的心有多慷慨了。

	talk
endproc

proc 310
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每个人都以为封印着风神的魂器早已失落在这世界上，可是一直有个秘密我没有告诉任何人。
	add_talk #b#b我的家族世代相传着一个秘密匣子，经过多年的调查，与家族流传下来的信息，我能肯定这个匣子就是装载着风神的魂器。
	add_talk #b#b虽然现在我已经一贫如洗，但匣子从未离开身边半步，如果你想了解更多匣子的事情，请把它当作我们之间的小秘密。另外…天上可是没有掉下来的馅饼呢，呵呵…你懂的。


	talk
endproc

proc 311
	dialog 1
	add_talk $_npc$:
	add_talk #b#b见你如此有诚意，又迫切着拯救世界的份上，作为新世纪有素质修养的流浪汉，我就把你当作命运之子吧。
	add_talk #b#b不过你还是得先帮我个忙，我的家族相传下来的神秘匣子，很有可能封印着风神，所以你还得帮我实现愿望，顺带证明你是足够拥有风神的力量。

	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	talk
endproc

;500
proc 501
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你是好人！	
	add_talk #b#b#
	add_talk #b#b#
	add_talk #b#b#W获得 #c186506[称号：善行者]#n
	talk
	;称号善行者
	var 1 0
	GET_PLAYER_ID 1 id
	HAS_ID_PLAYER_DESIGNATION @id 123 1
	if @1 != 1
		add_designation 1 123
	endif
endproc

proc 502
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这手笔有我当年的风范，想当初我也曾是个贵族。
	GET_PLAYER_ID 1 id
	;有钱人30分钟
	target_add_status 1 @id 3005 3 1800 1
	talk
endproc

proc 503
	dialog 1
	add_talk $_npc$:
	add_talk #b#b曾经我也家财万贯，直到一次次命运的捉弄，扒光了我所有财产，除了剩下的一个秘密匣子。它是我家族里代代相传下来的，既然至今它还留在我手上，或许是一种启示，冥冥中注定我将遇上某些转折。
	add_talk #b#b至于这个匣子的秘密，留下了这样一句话“命运之日，狂风将至”，似乎在传达匣子里装着什么不得了的东西。至于剩下的消息，我暂时只能说到这了，如果你还想了解些什么…呵呵…你懂的。
	talk
endproc

proc 504
	dialog 1
	add_talk $_npc$:
	add_talk #b#b传说中的风神象征着暴风！是唯一没有实体的精灵，只要是所到之处都将被它夷为平川！
	add_talk #b#b然而当风神被封印后，这个封印着它的魂器便失落在这世界上。有人说藏在了某个神殿中，有人说遗落在某个无人踏迹的地方，甚至有人说经过长年的岁月已经被摧毁了。
	add_talk #b#b但我说，这个封印着风神的魂器仍存在世界上，如果你还想知道更多，或许可以多来找找我，呵呵…你懂的。
	talk
endproc

proc 505
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我曾发誓，哪天遇见那位命运之子，若他助我实现愿望，就把家族世代相传的秘密匣子送给他。
	add_talk #b#b虽然你不失为旷世奇葩，但你现在的实力还不足以助我实现愿望， 70级后再来找我吧，若你就是命运安排的那个人，我会把一切都告诉你。
	talk
endproc

proc 506
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我曾发誓，哪天遇见那位命运之子，若他助我实现愿望，就把家族世代相传的秘密匣子送给他。
	add_talk #b#b我相信这个匣子仍留在我的身边，或许是一次命运的启示，反正我留着也没用。再怎么说我也是新世纪有素质修养的模范流浪汉，不过是不是命运安排就要看他的心有多慷慨了。
	talk
endproc

proc 507
	dialog 1
	add_talk $_npc$:
add_talk #b#b每个人都以为封印着风神的魂器早已失落在这世界上，可是一直有个秘密我没有告诉任何人。
add_talk #b#b我的家族世代相传着一个秘密匣子，经过多年的调查，与家族流传下来的信息，我能肯定这个匣子就是装载着风神的魂器。
add_talk #b#b虽然现在我已经一贫如洗，但匣子从未离开身边半步，如果你想了解更多匣子的事情，请把它当作我们之间的小秘密。另外…天上可是没有掉下来的馅饼呢，呵呵…你懂的。

	talk
endproc

proc 508
	dialog 1
	add_talk $_npc$:
	add_talk #b#b见你如此有诚意，又迫切着拯救世界的份上，作为新世纪有素质修养的流浪汉，我就把你当作命运之子吧。
	add_talk #b#b不过你还是得先帮我个忙，我的家族相传下来的神秘匣子，很有可能封印着风神，所以你还得帮我实现愿望，顺带证明你是足够拥有风神的力量。
	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	talk
endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b王者造梦，败者做梦，智者解梦，昧者迷梦，而我追梦！
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300916 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300916 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300917 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300918 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300919 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300920 1 0 0
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



; proc 810
	; rand 20 18
	; if 20 = 0
		; call 811
	; endif
	; if 20 = 1
		; call 812
	; endif
	; if 20 = 2
		; call 813
	; endif
	; if 20 = 3
		; call 814
	; endif
	; if 20 = 4
		; call 815
	; endif
	; if 20 = 5
		; call 816
	; endif
	; if 20 = 6
		; call 817
	; endif
	; if 20 = 7
		; call 818
	; endif
	; if 20 = 8
		; call 819
	; endif
	; if 20 = 9
		; call 820
	; endif
	; if 20 = 10
		; call 822
	; endif
	; if 20 = 11
		; call 811
	; endif
	; if 20 = 12
		; call 811
	; endif
	; if 20 = 13
		; call 811
	; endif
	; if 20 = 14
		; call 811
	; endif
	; if 20 = 15
		; call 811
	; endif
	; if 20 = 16
		; call 811
	; endif
	; if 20 = 17
		; call 811
	; endif
; endproc 
proc 811
	dialog 1
	add_talk $_npc$:
	add_talk #b#b告诉你个惊天秘密！我就说一遍，你要仔细听好！
	rand 20 18
	if 20 = 0
		add_talk #b#b据我所知，右侧快捷可以放置需要的道具，技能，方便战斗时候使用。
	endif
	if 20 = 1
		add_talk #b#b据我所知，在小地图上点击相应的位置，人物可以自动到达相应的地方。
	endif
	if 20 = 2
		add_talk #b#b据我所知，人物到达三十级时，可以前往时间城的“公会管理员”处，创建自己的公会。
	endif
	if 20 = 3
		add_talk #b#b据我所知，您可以在时间城的“公会管理员”处，通过“公会查询”功能，选择要加入的公会。
	endif
	if 20 = 4
		add_talk #b#b据我所知，您可以在“游戏设置”的“信息设置”中设置个人偏好的头顶显示信息。
	endif
	if 20 = 5
		add_talk #b#b据我所知，您可以在“游戏设置”的“按键设置”中设置个人偏好的快捷键。
	endif
	if 20 = 6
		add_talk #b#b据我所知，按F11键可以把玩家和宠物的名字隐藏起来！按F12键可以把摊位、宠物和队友以外的玩家外形隐藏起来！
	endif
	if 20 = 7
		add_talk #b#b据我所知，按TAB键会自动选中周围最近的怪物！
	endif
	if 20 = 8
		add_talk #b#b据我所知，按\键可以把界面隐藏起来！
	endif
	if 20 = 9
		add_talk #b#b据我所知，学会一项科技生产专业，你可以制造很多买不到的东西。
	endif
	if 20 = 10
		add_talk #b#b据我所知，大地之心连续闯过15层，可以获得1次转盘抽奖机会。
	endif
	if 20 = 11
		add_talk #b#b据我所知，使用“藏宝图”似乎能找到不错的宝石和大量银币。
	endif
	if 20 = 12
		add_talk #b#b据我所知，只有加入了公会，才能参加“驱逐入侵”“公会宝藏”“守护公会枢纽”“公会情报战”“公会据点战”“时间城争霸战”这些活动。
	endif
	if 20 = 13
		add_talk #b#b据我所知，击杀一定数量“可捕捉”的宠物后，将有可能碰上“可捕捉”的变异宠物！
	endif
	if 20 = 14
		add_talk #b#b据我所知，学习铸造专业后，可以在时间城的铸造大师处接取“铸造入门”任务，完成后将获得“采矿车”。
	endif
	if 20 = 15
		add_talk #b#b据我所知，学习炼金专业后，可以在时间城的炼金大师处接取“炼金入门”任务，完成后将获得“炼油车”。
	endif
	if 20 = 16
		add_talk #b#b据我所知，“藏宝图”“混乱元素”“欢乐嘉年华”这些活动中可以获得大量银币，千万不要错过了！
	endif
	if 20 = 17
		add_talk #b#b据我所知，搭载采集车，可以在地图上侦测到周围的矿点和油点！
	endif

	add_talk ……
	add_talk #b#b哎！别急！
	add_talk #b#b#c8B8B8B<流浪汉小心翼翼的掏出一个锦囊塞到你手中>#n
	add_talk #b#b如果还想要更多的锦囊！嘿嘿……你懂的。#n
	add_talk #b#b
	add_talk #c186506获得：神奇锦囊*1#n
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum	
	
	if jnum > 5
		sub jnum 5
		add_talk #c0040FF今日捐献次数：5/5 #n
		add_talk #c0040FF翡翠徽章特权捐献次数：$@jnum$/3 #n
	else
	
		add_talk #c0040FF今日捐献次数：$@jnum$/5 #n
		add_talk #c0040FF翡翠徽章特权额外捐献次数：0/3 #n
	
	endif
	
	get_role_var 31 8 1 10
	if 10 = 0
		add_talk #c0040FF水晶徽章特权免费次数：0/1 #n
	else
		add_talk #c0040FF水晶徽章特权免费次数：1/1 #n
	endif
	
	strvar yue ri
	GET_NOW_TIME yue ri 0 0 0 0 0
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		option 300 流浪汉的【感恩节】大回赠！！
	endif
	
	GOLD_RATE_GET_MONEY 10
	if 10 != -1
		if 10 >= 0 and 10 <= 4998
			option 801 捐献4998银币
		else
			option 800 捐献$@10$银币
		endif
	endif
	
	option 822 #我有水晶徽章特权，我来抢1个
	
	talk
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
			task_fini 4436 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b真是太感谢了，艾劳真是个漂亮姑娘！如果她愿意跟我约会，就真是太好了，我保证这会是我终身难忘的劳动节！
			option 511 #这个……我尽量……
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

proc 300
	dialog 1
	add_talk #b#b#b#b#b#b#b【感恩节】大回赠
	add_talk #b#b#b#b#b#b#b #c18650611月28日-12月2日#n
	add_talk #b#b
	; add_talk #b#b#b#b#b#b#b#b#c005ebe大回赠（一）
	; add_talk #b#b#b#b#b#b#b“双倍经验”状态#n
	; add_talk #b#b#b#b#b每天上线即可领取1次状态
	; add_talk #b#b状态效果：30分钟内杀怪获得双倍经验
	add_talk #b#b#b#b#b#b#b#b#c005ebe大回赠（一）
	add_talk #b#b#b#b#b#b “感恩之心”大礼包#n
	add_talk  每天累积在线1、2、4小时可分别领取1次礼包
	add_talk #b 打开礼包：将有机会获得“二级多彩宝石”
	
	get_player_daily_flag 10 16 11
	get_player_daily_flag 20 17 21
	get_player_daily_flag 30 18 31
	get_player_daily_flag 40 19 41
	STRVAR time
	GET_DAYONLINE 0 time
	
	; if 10 = 0 and 11 = 0
		; option 301 #领取“双倍经验”状态
	; endif
	if 20 = 0 and 21 = 0
		option 302 #领取“感恩之心”大礼包（累积在线1小时）
	endif
	if 30 = 0 and 31 = 0
		if time > 3600
			option 303 #领取“感恩之心”大礼包（累积在线2小时）
		endif
	endif
	if 40 = 0 and 41 = 0
		if time > 7200
			option 304 #领取“感恩之心”大礼包（累积在线4小时）
		endif
	endif
		
	talk
endproc

proc 301
	STRVAR ret num 
	
	get_player_daily_flag ret 16 num
	if ret = 0 and num = 0
		get_player_id 1 10
		TARGET_ADD_STATUS 1 @10 1039 1 1800 20
		
		strvar Mid
		GET_PLAYER_ID 0 Mid
		EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
		
		add_sys_msg_start
		add_sys_msg 流浪汉挥了挥衣袖，向你施加了1个”双倍经验“状态
		set_sys_msg_mode 3 0 0
		msg_info
		if 20 = 0
			var num 1
			SET_PLAYER_DAILY_FLAG 0 16 @num
		endif
	elseif ret = 0 and num = 1
		add_sys_msg_start
		add_sys_msg 你今天已经领取过了“双倍经验”状态，不能太贪心哦
		set_sys_msg_mode 3 0 0
		msg_info
	else
	endif
endproc

proc 302
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 3600
		get_player_daily_flag ret 17 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 1 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 流浪汉不急不忙的塞给你1个”感恩之心“大礼包
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 17 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出1格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线1小时的“感恩之心”大礼包，不能太贪心哦
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 3600
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 流浪汉仍在筹备大礼包中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 303
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 7200
		get_player_daily_flag ret 18 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 1 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 流浪汉不急不忙的塞给你1个”感恩之心“大礼包
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 18 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出1格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线2小时的“感恩之心”大礼包，不能太贪心哦
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 7200
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 流浪汉仍在筹备大礼包中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 304
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 14400
		get_player_daily_flag ret 19 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 1
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 2 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg 流浪汉不急不忙的塞给你2个”感恩之心“大礼包
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 19 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 请空出2格背包格子再前来领取
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg 你今天已领取过累积在线4小时的“感恩之心”大礼包，不能太贪心哦
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 14400
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG 流浪汉仍在筹备大礼包中，请$@10$分钟后再前来领取
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
