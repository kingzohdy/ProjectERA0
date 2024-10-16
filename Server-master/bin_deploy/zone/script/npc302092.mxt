;npc 跑商  洛克

proc 1
	GET_NOW_TIME 0 0 0 0 30 0 0
	if @30 >= 9 and @30 < 24
		strvar duihuanFlag
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b远道而来的商旅，来看看我这里有没有你想要的货物，也让我看看你都带来了些什么奇珍异宝，我会出高价收购它们！
		
		strvar sta sta_2 sta_3
		task_stat 4601 1 sta
		if sta = 2
			option 200 #你想要什么防具呢？
		endif
		task_stat 4602 1 sta_2
		if sta_2 = 2
			option 300 #你想要什么武器呢？
		endif
		task_stat 4603 1 sta_3
		if sta_3 = 2
			option 400 #你想要什么宠物蛋呢？
		endif
		
		TASK_STAT 4613 1 70
		if @70 = 2
			option 500 买卖贸易商品
			GET_PAOSHANG_INFO 6 duihuanFlag 0
			if @duihuanFlag = 0
				option 501 兑换贸易货币“银票”
			endif
		endif
			
		talk
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b早啊，伙计，你来的真是太早了，贸易商店还没开张呢，我每天都会在打烊的时候思考，怎样才能赚更多钱，怎样才能让自己的货物卖出更好的价格，可惜除了货比三家以外暂时还没找到更好的办法！我正考虑着是不是换两件更赚钱的货物呢，要不您帮我拿个主意？
        add_talk #b#b
		add_talk #b#b贸易商店开张时间 早上9：00—晚上23：59

		talk
	endif
endproc 

proc 500
	strvar nid nmemid
	GET_NPC_DEFID 0 nid
	GET_MAP_NPC_ID  @nid nmemid 10
	if @10 = 0
		;OPEN_PAOSHANG_UI 跑商定义表的id npc的memid 返回标志
		OPEN_PAOSHANG_UI 5 @nmemid 0
		PLAYER_CLOSE_DIALOG1 0 0 
	endif
endproc

proc 501
	strvar ret pglock
	PKG_IS_UNLOCK ret pglock
	if @ret = 0 and @pglock = 0
		return
	endif
	
	strvar maxhuanyp curryihuanyp 
	strvar yihaveitemnum yinpiao ExchangeFlag
	
    GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 3 yihaveitemnum 30
	GET_PAOSHANG_INFO 4 yinpiao 40
	
	if @10 = -1 or @20 = -1 or @30 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp > @maxhuanyp
		SET_PAOSHANG_INFO 4 1 0
		add_sys_msg_start
		add_sys_msg 兑换额度已满，你已拥有了足够多的本金，快去看看贸易商人出售的货物吧！
		set_sys_msg_mode 4 0 0
		msg_info
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk 兑换贸易货币——银票：
		add_talk #b#b#b最多可兑换银票：$@maxhuanyp$
		add_talk #b#b#b当前已兑换银票：$@curryihuanyp$
		add_talk #b#b
		add_talk 兑换比例：
		add_talk #b#b#b#c6d00d21银币 = 1银票#n
		
		option 603 使用银币兑换1000银票（1千银币）
		option 604 使用银币兑换5000银票（5千银币）
		option 605 返回上一页
		
		talk
	endif
endproc

proc 605
	call 1
endproc

;银币兑换1000银票（1千银币）
proc 603
	strvar maxhuanyp currhuanyp 
	strvar yinpiao tempnum jiaopiao
	strvar iflag
	var iflag 0
	
	GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 4 yinpiao 40
	GET_PAOSHANG_INFO 5 jiaopiao 50
	
	if @10 = -1 or @20 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp >= @maxhuanyp
		add_sys_msg_start
		add_sys_msg 兑换额度已满，你已拥有了足够多的本金，快去看看贸易商人出售的货物吧！
		set_sys_msg_mode 4 0 0
		msg_info
		call 1
		return
	endif
	
	var tempnum @curryihuanyp
	add tempnum 1000
	if @tempnum > @maxhuanyp
		add_sys_msg_start
		add_sys_msg 很抱歉，您的要求不合规矩，您剩余的额度不足
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 1000 20 15
	if 15 = 0
		DEC_MONEY 0 1000 20 12
		if 12 = 0		
			add yinpiao 1000
			SET_PAOSHANG_INFO 2 @yinpiao 60
			add curryihuanyp 1000
			SET_PAOSHANG_INFO 1 @curryihuanyp 70
			
			PAOSHANG_DUIHUAN_REFRESH 1 0

			if @curryihuanyp >= @maxhuanyp
				SET_PAOSHANG_INFO 4 1 0
				add_sys_msg_start
				add_sys_msg 兑换额度已满，你已拥有了足够多的本金，快去看看贸易商人出售的货物吧！
				set_sys_msg_mode 4 0 0
				msg_info
				var iflag 1
			endif
			
			if @jiaopiao <= @yinpiao
				TASK_STAT 4613 1 80
				if @80 = 2
					TASK_FINI 4613 0
					var iflag 1
				endif
			endif
			
			if @iflag = 1
				call 1
			else
				call 501
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif			
endproc

;银币兑换5000银票（5千银币）
proc 604
	strvar maxhuanyp currhuanyp 
	strvar yinpiao tempnum jiaopiao
	strvar iflag
	var iflag 0
	
	GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 4 yinpiao 40
	
	GET_PAOSHANG_INFO 5 jiaopiao 50
	
	if @10 = -1 or @20 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp >= @maxhuanyp
		add_sys_msg_start
		add_sys_msg 兑换额度已满，你已拥有了足够多的本金，快去看看贸易商人出售的货物吧！
		set_sys_msg_mode 4 0 0
		msg_info
		call 1
		return
	endif
	
	var tempnum @curryihuanyp
	add tempnum 5000
	if @tempnum > @maxhuanyp
		add_sys_msg_start
		add_sys_msg 很抱歉，您的要求不合规矩，您剩余的额度不足
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 5000 20 15
	if 15 = 0
		DEC_MONEY 0 5000 20 12
		if 12 = 0		
			add yinpiao 5000
			SET_PAOSHANG_INFO 2 @yinpiao 60
			add curryihuanyp 5000
			SET_PAOSHANG_INFO 1 @curryihuanyp 70
			
			PAOSHANG_DUIHUAN_REFRESH 1 0
			
			if @curryihuanyp >= @maxhuanyp
				SET_PAOSHANG_INFO 4 1 0
				add_sys_msg_start
				add_sys_msg 兑换额度已满，你已拥有了足够多的本金，快去看看贸易商人出售的货物吧！
				set_sys_msg_mode 4 0 0
				msg_info
				var iflag 1
			endif
			
			if @jiaopiao <= @yinpiao
				TASK_STAT 4613 1 80
				if @80 = 2
					TASK_FINI 4613 0
					var iflag 1
				endif
			endif
			
			if @iflag = 1
				call 1
			else
				call 501
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif			
endproc


proc 200
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4601 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一件#c186506白色品质的野战服#n，可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一件#c186506白色品质的亚麻衣#n，可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一件#c186506白色品质的狂战装#n，可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一件#c186506白色品质的祷告装#n，可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一件#c186506白色品质的精铁头盔#n，可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一件#c186506白色品质的符文头巾#n，可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一件#c186506白色品质的优雅软帽#n，可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一件#c186506白色品质的十字纹饰#n，可以不？
	endif
	if ret = 8
		add_talk #b#b我想要一件#c186506白色品质的武装面具#n，可以不？
	endif
	if ret = 9
		add_talk #b#b我想要一件#c186506白色品质的元素护饰#n，可以不？
	endif
	if ret = 10
		add_talk #b#b我想要一件#c186506白色品质的热诚面饰#n，可以不？
	endif
	if ret = 11
		add_talk #b#b我想要一件#c186506白色品质的教徒面具#n，可以不？
	endif
	if ret = 12
		add_talk #b#b我想要一件#c186506白色品质的精铁肩甲#n，可以不？
	endif
	if ret = 13
		add_talk #b#b我想要一件#c186506白色品质的符文肩甲#n，可以不？
	endif
	if ret = 14
		add_talk #b#b我想要一件#c186506白色品质的野火II型#n，可以不？
	endif
	if ret = 15
		add_talk #b#b我想要一件#c186506白色品质的十字肩甲#n，可以不？
	endif
	if ret = 16
		add_talk #b#b我想要一件#c186506白色品质的战士靴#n，可以不？
	endif
	if ret = 17
		add_talk #b#b我想要一件#c186506白色品质的符文靴#n，可以不？
	endif
	if ret = 18
		add_talk #b#b我想要一件#c186506白色品质的冒险靴#n，可以不？
	endif
	if ret = 19
		add_talk #b#b我想要一件#c186506白色品质的主教靴#n，可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 201

	talk
endproc  

proc 201
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4601 0 8 1 ret 
	if ret = 0
		if ID = 2019902
			DEC_ITEM_NUM_BYIDX 2019902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的野战服？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 2029902
			DEC_ITEM_NUM_BYIDX 2029902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的亚麻衣？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 2039902
			DEC_ITEM_NUM_BYIDX 2039902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的狂战装？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 2049902
			DEC_ITEM_NUM_BYIDX 2049902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的祷告装？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 2119902
			DEC_ITEM_NUM_BYIDX 2119902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精铁头盔？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 2129902
			DEC_ITEM_NUM_BYIDX 2129902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文头巾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 2139902
			DEC_ITEM_NUM_BYIDX 2139902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的优雅软帽？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 2149902
			DEC_ITEM_NUM_BYIDX 2149902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字纹饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 2419902
			DEC_ITEM_NUM_BYIDX 2419902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的武装面具？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 2429902
			DEC_ITEM_NUM_BYIDX 2429902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的元素护饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 2439902
			DEC_ITEM_NUM_BYIDX 2439902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的热诚面饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 2449902
			DEC_ITEM_NUM_BYIDX 2449902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的教徒面具？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 2219902
			DEC_ITEM_NUM_BYIDX 2219902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精铁肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 2229902
			DEC_ITEM_NUM_BYIDX 2229902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1279902
			DEC_ITEM_NUM_BYIDX 1279902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的野火II型？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 2249902
			DEC_ITEM_NUM_BYIDX 2249902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 2619902
			DEC_ITEM_NUM_BYIDX 2619902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的战士靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 2629902
			DEC_ITEM_NUM_BYIDX 2629902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 2639902
			DEC_ITEM_NUM_BYIDX 2639902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的冒险靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 2649902
			DEC_ITEM_NUM_BYIDX 2649902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的主教靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 300
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4602 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一把#c186506白色品质的金刚刀#n，可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一把#c186506白色品质的精金刀#n，可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一把#c186506白色品质的骑士刀#n，可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一把#c186506白色品质的血腥锋芒#n，可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一把#c186506白色品质的复仇利刃#n，可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一把#c186506白色品质的秘银盾#n，可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一把#c186506白色品质的白金盾#n，可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一把#c186506白色品质的龙骨盾#n，可以不？
	endif
	if ret = 8
		add_talk #b#b我想要一把#c186506白色品质的狮鹫之盾#n，可以不？
	endif
	if ret = 9
		add_talk #b#b我想要一把#c186506白色品质的力量庇佑#n，可以不？
	endif
	if ret = 10
		add_talk #b#b我想要一把#c186506白色品质的赤杨杖#n，可以不？
	endif
	if ret = 11
		add_talk #b#b我想要一把#c186506白色品质的黑木杖#n，可以不？
	endif
	if ret = 12
		add_talk #b#b我想要一把#c186506白色品质的熔岩杖#n，可以不？
	endif
	if ret = 13
		add_talk #b#b我想要一把#c186506白色品质的战火之杖#n，可以不？
	endif
	if ret = 14
		add_talk #b#b我想要一把#c186506白色品质的光明权杖#n，可以不？
	endif
	if ret = 15
		add_talk #b#b我想要一把#c186506白色品质的洞察烛#n，可以不？
	endif
	if ret = 16
		add_talk #b#b我想要一把#c186506白色品质的血色薇#n，可以不？
	endif
	if ret = 17
		add_talk #b#b我想要一把#c186506白色品质的灵魂印记#n，可以不？
	endif
	if ret = 18
		add_talk #b#b我想要一把#c186506白色品质的精神深渊#n，可以不？
	endif
	if ret = 19
		add_talk #b#b我想要一把#c186506白色品质的合金指纹#n，可以不？
	endif
	if ret = 20
		add_talk #b#b我想要一把#c186506白色品质的铁膛II#n，可以不？
	endif
	if ret = 21
		add_talk #b#b我想要一把#c186506白色品质的铁膛III#n，可以不？
	endif
	if ret = 22
		add_talk #b#b我想要一把#c186506白色品质的涡轮I#n，可以不？
	endif
	if ret = 23
		add_talk #b#b我想要一把#c186506白色品质的涡轮II#n，可以不？
	endif
	if ret = 24
		add_talk #b#b我想要一把#c186506白色品质的涡轮III#n，可以不？
	endif
	if ret = 25
		add_talk #b#b我想要一把#c186506白色品质的重锤II#n，可以不？
	endif
	if ret = 26
		add_talk #b#b我想要一把#c186506白色品质的重锤III#n，可以不？
	endif
	if ret = 27
		add_talk #b#b我想要一把#c186506白色品质的螺旋I#n，可以不？
	endif
	if ret = 28
		add_talk #b#b我想要一把#c186506白色品质的螺旋II#n，可以不？
	endif
	if ret = 29
		add_talk #b#b我想要一把#c186506白色品质的螺旋III#n，可以不？
	endif
	if ret = 30
		add_talk #b#b我想要一把#c186506白色品质的铭文杖#n，可以不？
	endif
	if ret = 31
		add_talk #b#b我想要一把#c186506白色品质的修士杖#n，可以不？
	endif
	if ret = 32
		add_talk #b#b我想要一把#c186506白色品质的幻影杖#n，可以不？
	endif
	if ret = 33
		add_talk #b#b我想要一把#c186506白色品质的祈祷之杖#n，可以不？
	endif
	if ret = 34
		add_talk #b#b我想要一把#c186506白色品质的法典权杖#n，可以不？
	endif
	if ret = 35
		add_talk #b#b我想要一把#c186506白色品质的星光风#n，可以不？
	endif
	if ret = 36
		add_talk #b#b我想要一把#c186506白色品质的圣律典#n，可以不？
	endif
	if ret = 37
		add_talk #b#b我想要一把#c186506白色品质的圣法阵#n，可以不？
	endif
	if ret = 38
		add_talk #b#b我想要一把#c186506白色品质的灵魂吐息#n，可以不？
	endif
	if ret = 39
		add_talk #b#b我想要一把#c186506白色品质的十字戒令#n，可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 301

	talk
endproc  

proc 301
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4602 0 8 1 ret 
	if ret = 0
		if ID = 1019902
			DEC_ITEM_NUM_BYIDX 1019902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的金刚刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 1019903
			DEC_ITEM_NUM_BYIDX 1019903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精金刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 1019904
			DEC_ITEM_NUM_BYIDX 1019904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的骑士刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 1019905
			DEC_ITEM_NUM_BYIDX 1019905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的血腥锋芒？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 1019906
			DEC_ITEM_NUM_BYIDX 1019906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的复仇利刃？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 1069902
			DEC_ITEM_NUM_BYIDX 1069902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的秘银盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 1069903
			DEC_ITEM_NUM_BYIDX 1069903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的白金盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 1069904
			DEC_ITEM_NUM_BYIDX 1069904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的龙骨盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 1069905
			DEC_ITEM_NUM_BYIDX 1069905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的狮鹫之盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 1069906
			DEC_ITEM_NUM_BYIDX 1069906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的力量庇佑？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 1109902
			DEC_ITEM_NUM_BYIDX 1109902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的赤杨杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 1109903
			DEC_ITEM_NUM_BYIDX 1109903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的黑木杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 1109904
			DEC_ITEM_NUM_BYIDX 1109904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的熔岩杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 1109905
			DEC_ITEM_NUM_BYIDX 1109905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的战火之杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1109906
			DEC_ITEM_NUM_BYIDX 1109906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的光明权杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 1189902
			DEC_ITEM_NUM_BYIDX 1189902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的洞察烛？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 1189903
			DEC_ITEM_NUM_BYIDX 1189903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的血色薇？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 1189904
			DEC_ITEM_NUM_BYIDX 1189904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的灵魂印记？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 1189905
			DEC_ITEM_NUM_BYIDX 1189905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精神深渊？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 1189906
			DEC_ITEM_NUM_BYIDX 1189906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的合金指纹？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 20
		if ID = 1209902
			DEC_ITEM_NUM_BYIDX 1209902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铁膛II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 21
		if ID = 1209903
			DEC_ITEM_NUM_BYIDX 1209903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铁膛III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 22
		if ID = 1209904
			DEC_ITEM_NUM_BYIDX 1209904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮I？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 23
		if ID = 1209905
			DEC_ITEM_NUM_BYIDX 1209905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 24
		if ID = 1209906
			DEC_ITEM_NUM_BYIDX 1209906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 25
		if ID = 1289902
			DEC_ITEM_NUM_BYIDX 1289902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的重锤II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 26
		if ID = 1289903
			DEC_ITEM_NUM_BYIDX 1289903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的重锤III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 27
		if ID = 1289904
			DEC_ITEM_NUM_BYIDX 1289904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋I？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 28
		if ID = 1289905
			DEC_ITEM_NUM_BYIDX 1289905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 29
		if ID = 1289906
			DEC_ITEM_NUM_BYIDX 1289906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 30
		if ID = 1509902
			DEC_ITEM_NUM_BYIDX 1509902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铭文杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 31
		if ID = 1509903
			DEC_ITEM_NUM_BYIDX 1509903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的修士杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 32
		if ID = 1509904
			DEC_ITEM_NUM_BYIDX 1509904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的幻影杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 33
		if ID = 1509905
			DEC_ITEM_NUM_BYIDX 1509905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的祈祷之杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 34
		if ID = 1509906
			DEC_ITEM_NUM_BYIDX 1509906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的法典权杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 35
		if ID = 1589902
			DEC_ITEM_NUM_BYIDX 1589902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的星光风？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 36
		if ID = 1589903
			DEC_ITEM_NUM_BYIDX 1589903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的圣律典？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
	endif
	if ret = 37
		if ID = 1589904
			DEC_ITEM_NUM_BYIDX 1589904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的圣法阵？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 38
		if ID = 1589905
			DEC_ITEM_NUM_BYIDX 1589905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的灵魂吐息？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 39
		if ID = 1589906
			DEC_ITEM_NUM_BYIDX 1589906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字戒令？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 400
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4603 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一只#c186506嘟嘟鸟宠物蛋#n，可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一只#c186506野猪宠物蛋#n，可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一只#c186506孢子人宠物蛋#n，可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一只#c186506沼泽巨鳄宠物蛋#n，可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一只#c186506秃鹫宠物蛋#n，可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一只#c186506幽光幼龙宠物蛋#n，可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一只#c186506剑齿虎宠物蛋#n，可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一只#c186506蝎子宠物蛋#n，可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 401

	talk
endproc  

proc 401
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4603 0 8 1 ret 
	if ret = 0
		if ID = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是嘟嘟鸟宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是野猪宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是孢子人宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是沼泽巨鳄宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是秃鹫宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是幽光幼龙宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是剑齿虎宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 5610105
			DEC_ITEM_NUM_BYIDX 5610105 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是蝎子宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 101
	add_sys_msg_start
;	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

