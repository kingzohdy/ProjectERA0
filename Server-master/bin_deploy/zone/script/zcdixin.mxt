;大地之心NPC

proc 1

	
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b如果我们像小蜗牛一样，一遇到风吹草动马上就躲进壳里，那怎么还能体会到生活的情趣呢？
	add_talk #b#b我们应该不断去探索，勇敢的接受生活的挑战，这样才算多姿多彩的人生。
	add_talk #b#b现在就有一个机会摆在你面前，愿意随我进行一次穿越地心的探险旅行么？
	add_talk #b#b
	add_talk #c186506特别提示：
	add_talk #b#b进入或离开大地之心，将自动消除身上的状态
	
	task_add_option 3341 1 100
	task_add_option 3342 1 101
	task_add_option 3170 1 102
	task_add_option 3171 1 103
	
	task_stat 3410 1 30
	if 30 = 1
		task_add_option 3410 1 301
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300927 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	task_stat 3401 1 2
	task_stat 3410 1 3
	if @2 = 2 or @3 = 2
		option 230 #进入大地之心
		
		option 240 #布罗克教授哪里去了？
		option 225 15层太难了，还有其他办法吗？
		
	talk
	else
		option 230 #进入大地之心
	endif
	
	
	talk	
endproc 

PROC 100
	task_do_option 3341 1
ENDPROC
PROC 101
	task_do_option 3342 1
ENDPROC
PROC 102
	task_do_option 3170 1
ENDPROC
PROC 103
	task_do_option 3171 1
ENDPROC
proc 2
	get_trig_task 1 2 
	if 2 = 3171
		task_fini 3171 1
	endif

endproc 
proc 225
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b当然有，要帮你开张证明也不是什么难事，只不过嘛……
	add_talk #b#b#c676464<凡尔纳半眯着眼睛上上下下地打量着你，右手举在胸前，手指与食指、中指做出捏的动作，然后轻飘飘地说>#n
	add_talk #b#b这件事情是冒了很大风险的……而且，我还需要上下打点打点……
	
		option 226 #我愿意出2千银币，只求一纸证明
		option 300 #还有其他办法吗？
	talk
endproc
proc 226
	;strvar Moy
	;DEC_MONEY_TEST 0 20 20 Moy
	;if Moy = -1
		;ADD_SYS_MSG_START
		;ADD_SYS_MSG 很抱歉，你背包没有那么多钱。
		;SET_SYS_MSG_MODE 1 0 0
		;MSG_INFO
		;return
	;else
		strvar Num
		get_pack_space 0 Num
		if Num > 0
		
			DEC_MONEY_TEST 0 2000 20 30
			if 30 = 0
				ADD_ITEM_NUM_INPAK 5300522 1 0 10
				if 10 = 0
					DEC_MONEY 0 2000 20 10
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG 你好像没有那么多钱！
				SET_SYS_MSG_MODE 4 0 0
				MSG_INFO
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包剩余空间不足！
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		endif
	;endif
	
endproc

proc 230
	OPEN_UI 36
endproc

proc 240
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b布罗克教授为大地之心做出了非常巨大的贡献！
	add_talk #b#b为了大地心之能够正常运转，他现在正在第15层做常规检查与维护，如果你有很重要的事情，就到那里去找他吧。
	

		option 250 #请送我进入大地之心
		option 260 #等我更强力了再去找他
	talk

endproc

proc 250
	OPEN_UI 36
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b俗话说，有钱的出钱，有力的出力，如今我刚好有件小事请你帮忙，不用紧张，不是什么大事。
	add_talk #b#b前两天，我去越雾通道旅游时被火雨好战者袭击了，等我回来时，发现有张彩票不见了，然后，我接到通知，说那张彩票中了特等奖…
	add_talk #b#b只要你能帮我把彩票找回来，我就会无条件的赠送你一张布罗克教授的证明。
	add_talk #b#b如果我是你，我现在就不会站在这里发呆。
	
	add_talk #b#b 时间就是金钱，$_Career$！
	
		task_add_option 3410 1 301
	talk
endproc

PROC 301
	task_do_option 3410 1
ENDPROC

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，这位同学，好久不见啊，我借给你的《雷蒙盖顿传奇》你终于舍得还给我啦，什么，不是还是换？
	add_talk #b#b好吧，我这里还有其他章节，让我找一找。

		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300927 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300927 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300928 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300929 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300931 1 0 0
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

