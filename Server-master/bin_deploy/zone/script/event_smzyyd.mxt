;生命在于运动   事件ID:54     活动助手ID:96

PROC 1
	;时间第几秒。该出现哪句话
	SET_EVENT_VAR 0 32 0 1
	;要冒泡活动介绍还是颁布指令    1为颁布指令   0则执行冒泡活动介绍
	SET_EVENT_VAR 4 32 0 1
	;proc 202 中，0 执行rand   1执行另一些代码...还有proc 300 ~ proc 313 中 该执行哪句mon_speech 0第一句，1第二句
	SET_EVENT_VAR 8 32 0 1
	;记录是什么动作   0 ~ 13  分别对应不同的动作
	strvar Loc
	var Loc 0
	SET_EVENT_VAR 12 32 @Loc 1
	
	;在海边集体做表情动作，共25次  
	SET_EVENT_VAR 16 32 0 1
	
	;假如有100个人做对了，就刷宝箱
	SET_EVENT_VAR 20 32 0 1
	
	strvar time 
	GET_TIME_SEC time
	SET_EVENT_VAR 24 32 @time 1
	
	;记录标志怪的坐标X,Y
	SET_EVENT_VAR 28 32 0 1
	SET_EVENT_VAR 32 32 0 1
		
	add_sys_msg_start
	add_sys_msg 生命在于运动，亲爱的朋友们，罗拉在阳光海滩等待着与您一起做运动哟！
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

PROC 2		
	add_sys_msg_start
	add_sys_msg 适当的运动能够强身健体，保持完美身材，本次运动到此结束，罗拉感谢各位的陪伴！
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

;10秒触发一次
PROC 3	
	GET_EVENT_VAR 16 32 1 30
	if @30 > 25
		return
	endif

	GET_EVENT_VAR 4 32 1 20
	if @20 = 0
		call 201
	else 
	   call 202
	endif
ENDPROC

proc 201
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	GET_EVENT_VAR 0 32 1 50
	if @50 = 0
		MON_SPEECH  @npcmid 0 2 1 欢迎各位同学来跟人家一起做运动，请先放松身心，听人家讲讲运动技巧。#010#010#003#003
	elseif @50 = 1
		MON_SPEECH  @npcmid 0 2 1 只要点击#G聊天窗口右下角的笑脸图标#n，就可以打开动作指令面板。#037#022
	elseif @50 = 2
		MON_SPEECH  @npcmid 0 2 1 #G点击动作面板中的动作指令图标#n，就可以做出相应的动作了！#003#023
	elseif @50 = 3
		MON_SPEECH  @npcmid 0 2 1 稍后人家附近会出现炫丽的灯光，人家只会看到在灯光下的同学！#028#003
	elseif @50 = 4
		MON_SPEECH  @npcmid 0 2 1 在灯光下每做对一个动作，都会获得奖励，所以要集中注意力哦。#022#022
	elseif @50 = 5
		MON_SPEECH  @npcmid 0 2 1 介绍完毕，若您还不明白，可以靠近点查看人家的“运动技巧”介绍。#003#003#003
	elseif @50 = 6
		MON_SPEECH  @npcmid 0 2 1 准备好了吗？亲爱的，人家已经开始等不及与你们一起做运动了！#022#037#010
	elseif @50 = 7
		MON_SPEECH  @npcmid 0 2 1 预备…人家要开始提示动作了…#056#056#056
		SET_EVENT_VAR 4 32 1 1
	else
		return
	endif
	
	add 50 1
	SET_EVENT_VAR 0 32 @50 1
endproc


proc 202	
	GET_EVENT_VAR 16 32 1 30
	if @30 > 25
		return
	endif
	
	GET_EVENT_VAR 8 32 1 21
	if @21 = 0			
		rand 12 14
		add 12 1
		switch 12
			case 1
				SET_EVENT_VAR 12 32 1 1
				call 301
			endcase

			case 2
				SET_EVENT_VAR 12 32 2 1
				call 302
			endcase
					
			case 3
				SET_EVENT_VAR 12 32 3 1
				call 303
			endcase
					
			case 4
				SET_EVENT_VAR 12 32 4 1
				call 304
			endcase
					
			case 5
				SET_EVENT_VAR 12 32 5 1
				call 305
			endcase
				
			case 6
				SET_EVENT_VAR 12 32 6 1
				call 306
			endcase
					
			case 7
				SET_EVENT_VAR 12 32 7 1
				call 307
			endcase
					
			case 8
				SET_EVENT_VAR 12 32 8 1
				call 308
			endcase
					
			case 9
				SET_EVENT_VAR 12 32 9 1
				call 309
			endcase

			case 10
				SET_EVENT_VAR 12 32 10 1
				call 310
			endcase
					
			case 11
				SET_EVENT_VAR 12 32 11 1
				call 311
			endcase
					
			case 12
	    		SET_EVENT_VAR 12 32 12 1
				call 312
			endcase
					
			case 13
	    		SET_EVENT_VAR 12 32 13 1
				call 313
	    	endcase
			
			case 14
				SET_EVENT_VAR 12 32 14 1
				call 300
			endcase
		endswitch
		
		SET_EVENT_VAR 8 32 1 1
		
		;第几个动作
		GET_EVENT_VAR 16 32 1 30
		add 30 1
		SET_EVENT_VAR 16 32 @30 1
	else		
		strvar Loc
		GET_EVENT_VAR 12 32 1 Loc
		if @Loc = 1
			call 301
		elseif @Loc = 2
			call 302
		elseif @Loc = 3
			call 303
		elseif @Loc = 4
			call 304
		elseif @Loc = 5
			call 305
		elseif @Loc = 6
			call 306
		elseif @Loc = 7
			call 307
		elseif @Loc = 8
			call 308
		elseif @Loc = 9
			call 309
		elseif @Loc = 10
			call 310
		elseif @Loc = 11
			call 311
		elseif @Loc = 12
			call 312
		elseif @Loc = 13
			call 313
		elseif @Loc = 14
			call 300
		else
			return
		endif
		
		SET_EVENT_VAR 8 32 0 1
		
		;记录标志怪的坐标X,Y   清0
		SET_EVENT_VAR 28 32 0 1
		SET_EVENT_VAR 32 32 0 1
		
		;最多25次动作可做
		GET_EVENT_VAR 16 32 1 30
		
		if @30 >= 25
			GET_MAP_INST 4403 10
			strvar npcmid
			GET_MAP_NPC_ID  304125 npcmid 1
			MON_SPEECH  @npcmid 0 2 1 本次运动到此结束，亲爱的，希望下次你还会再来陪我做运动。#006#006#006
			
			add 30 1
			SET_EVENT_VAR 16 32 @30 1
			return
		endif
	endif
endproc

proc 300 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G欢呼#n，开始！#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “欢呼”结束！下一个动作即将开始！#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 301 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G飞吻#n，开始！#022#022#022#022
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “飞吻”结束！下一个动作即将开始！#022#022#022#022
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 302 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G跳舞#n，开始！#009#009#009#009
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “跳舞”结束！下一个动作即将开始！#009#009#009#009
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 303 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G鼓舞#n，开始！#023#023#023#023
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “鼓舞”结束！下一个动作即将开始！#023#023#023#023
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 304 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G装死#n，开始！#031#031#031#031
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “装死”结束！下一个动作即将开始！#031#031#031#031
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 305 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G攻击#n，开始！#037#037#037#037
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “攻击”结束！下一个动作即将开始！#037#037#037#037
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 306 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G鼓掌#n，开始！#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else	
		mon_speech @npcmid 0 2 1 “鼓掌”结束！下一个动作即将开始！#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 307 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G耍酷#n，开始！#003#003#003#003
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “耍酷”结束！下一个动作即将开始！#003#003#003#003
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 308 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G乞讨#n，开始！#009#009#009#009
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “乞讨”结束！下一个动作即将开始！#009#009#009#009
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 309 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G作揖#n，开始！#004#004#004#004
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “作揖”结束！下一个动作即将开始！#004#004#004#004
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 310 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G无奈#n，开始！#056#056#056#056
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “无奈”结束！下一个动作即将开始！#056#056#056#056
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 311 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2 Loc3
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G挥手#n，开始！#022#022#022#022
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “挥手”结束！下一个动作即将开始！#022#022#022#022
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 312 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G深思#n，开始！#002#002#002#002
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “深思”结束！下一个动作即将开始！#002#002#002#002
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

proc 313 
	GET_MAP_INST 4403 10
	strvar npcmid
	GET_MAP_NPC_ID  304125 npcmid 1
	
	strvar Loc2
	GET_EVENT_VAR 8 32 1 Loc2
	if @Loc2 = 0
		mon_speech @npcmid 0 2 1 #G痛苦#n，开始！#012#012#012#012
		
		strvar outx outy
		GET_MAP_INST 4403 10
		;罗拉坐标22475 15738   的12米之内，刷标志怪
		GET_MAP_RANDPOINT  22475 15738 1200 56 outx outy 
		;功能: 获取某个地图坐标的某个半径范围内的随机坐标
		if @56 = 0
			NEW_LIFE_MON 203803 1 @outx @outy 0 0 0 0 10 0 0
			SET_EVENT_VAR 28 32 @outx 1
			SET_EVENT_VAR 32 32 @outy 1
		endif
		
		SET_EVENT_VAR 8 32 1 1
	else
		mon_speech @npcmid 0 2 1 “痛苦”结束！下一个动作即将开始！#012#012#012#012
		SET_EVENT_VAR 8 32 0 1
		;累积的同时做对的人数，下个动作开始前，应该清0
		SET_EVENT_VAR 20 32 0 1
	endif
endproc

;动作判断
proc 5
	GET_EVENT_STAT 10 96
	if @10 = 0
	;活动未开始
		return
	endif
	
	strvar roletime begintime tmptime
	GET_ROLE_VAR 52 32 1 roletime
	
	;事件开始的时间
	GET_EVENT_VAR 24 32 1 begintime
	var tmptime @begintime
	;15分钟
	add tmptime 900
	if @roletime < @begintime or @roletime > @tmptime
		set_role_var 52 32 @begintime 1
		;清为0
		set_role_var 51 8 0 1
		;56位代表记录该角色总共做对的次数
		set_role_var 56 8 0 1
		;57位代表该角色在做对25次动作的情况下是否去罗拉那里领取奖励，1表示可以领，0不行
		;set_role_var 57 8 0 1
	endif
	
	GET_EVENT_VAR 4 32 1 26
	;0的话。表示还没颁布动作命令
	if @26 = 0
		return
	endif
	
	;当前动作
	GET_EVENT_VAR 16 32 1 30

	strvar Ret
	;角色记录动作
	get_role_var 51 8 Ret 31
	if @Ret < 0
		return
	endif
	
	if @30 = @31
		return
	endif
	
	GET_EVENT_VAR 28 32 1 87
	GET_EVENT_VAR 32 32 1 88
	strvar x y m
	;获取玩家坐标
    GET_PLAYER_MAPINFO 1 0 0 x y
	;在这个坐标的3米内都算完成
	GET_POINT_DIST @x @y @87 @88 m
	if m > 300
		add_sys_msg_start
		add_sys_msg 只有在正确的时间（罗拉的口令）正确的地点（镁光灯柱下）做出正确的动作才有奖励哟！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar MotIon Loc 
	ID_PLAYER_MOTION 0 0 MotIon          

	GET_EVENT_VAR 12 32 1 Loc
	if @Loc = 1
		if MotIon = 10701
		;飞吻		
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 2
		if MotIon = 10702
		;跳舞
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 3
		if MotIon = 10703
		;鼓舞
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 4
		if MotIon = 10704
		;装死
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 5
		if MotIon = 10705
		;攻击
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 6
		if MotIon = 10706
		;鼓掌
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 7
		if MotIon = 10707
		;耍酷
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 8
		if MotIon = 10708
		;乞讨
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
		
	if @Loc = 9
		if MotIon = 10709
		;作揖
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 10
		if MotIon = 10710
		;无奈
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 11
		if MotIon = 10711
		;挥手
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 12
		if MotIon = 10712
		;深思
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 13
		if MotIon = 10713
		;痛苦
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
	
	if @Loc = 14
		if MotIon = 10700
		;欢呼			
			;call 402
			
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			;记录该玩家角色做对的次数
			strvar rrRet
			get_role_var 56 8 rrRet 99
			if @rrRet = 0 
				add 99 1
				set_role_var 56 8 @99 1
				if @99 >= 25
					;做对25次，设置为1，表示可以领取奖品
					set_role_var 57 8 1 1
					add_sys_msg_start
					add_sys_msg 由于你出色的表现获得了特殊称号，请尽快前往罗拉处领取
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
			
			call 401
		else	
			;当前动作
			GET_EVENT_VAR 16 32 1 40
			if @40 < 0 or @40 > 26
				return
			else
			;不能重复单击当前动作
				set_role_var 51 8 @40 1
			endif
			
			call 400
		endif
	endif
endproc

proc 400
	add_sys_msg_start
	add_sys_msg 你有没有一种鹤立鸡群的感觉呢？同学，你的动作做错了哟！
	set_sys_msg_mode 3 0 0
	msg_info
	
	strvar aflag
	GET_PLAYER_ID 1 aflag
	if @aflag = -1
		return
	else
		;给目标添加状态
		TARGET_ADD_STATUS 1 @aflag 1057 1 2 90
	endif
endproc


;每做对一次，给5060050 
proc 401
	add_sys_msg_start
	add_sys_msg 非常完美，您这动作绝对的明星范！
	set_sys_msg_mode 3 0 0
	msg_info
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 您的背包已经装不下更多东西了
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	rand 55 100
	if @55 <= 77 and @55 >= 0
		ADD_BIND_ITEM_NUM_INPAK 5060050 1 1 14 
	else
		ADD_BIND_ITEM_NUM_INPAK 5060050 10 1 14 
		add_sys_msg_start
		add_sys_msg  $_player$在阳光海滩与罗拉一起运动的过程中表现出色，获得了女神的青睐！
		set_sys_msg_mode 18 0 0
		SVR_MSG_INFO 
	endif
endproc

proc 402
	strvar x1 y1 PeoNum
	var PeoNum 2
	rand 15 8
	switch 15
		case 0
			var x1 14509
			var y1 29289
		endcase

		case 1
			var x1 15078
			var y1 29395
		endcase

		case 2
			var x1 15482
			var y1 29055
		endcase
				
		case 3
			var x1 15625
			var y1 28610
		endcase
				
		case 4
			var x1 15413
			var y1 28247
		endcase
		
		case 5
			var x1 14995
			var y1 28114
		endcase
		
		case 6
			var x1 14562
			var y1 28285
		endcase
		
		case 7
			var x1 14285
			var y1 28742
		endcase
	endswitch
	
	GET_EVENT_VAR 20 32 1 60
	add 60 1
	SET_EVENT_VAR 20 32 @60 1
	
	if @60 >= @PeoNum
		NEW_LIFE_MON 293524 10 @x1 @y1 800 0 0 0 20 0 0
		add_sys_msg_start
		add_sys_msg 为了感谢大家的积极配合，罗拉抛洒出了女神的青睐，其中有超级幸运奖品哟！
		set_sys_msg_mode 4 0 0
		SVR_MSG_INFO
	endif
endproc
