;npc 跑商  拉斐特

proc 1
	GET_NOW_TIME 0 0 0 0 30 0 0
	if @30 >= 9 and @30 < 24
		strvar duihuanFlag
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b远道而来的商旅，来看看我这里有没有你想要的货物，也让我看看你都带来了些什么奇珍异宝，我会出高价收购它们！
		
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
		OPEN_PAOSHANG_UI 3 @nmemid 0
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
