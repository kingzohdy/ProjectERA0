




proc 1
	

	;DEC_ITEM_NUM_INPAK 5241520 1 100
	;if 100 = 0

		;GET_PLAYER_ID 1 Mid
		;EFFECT_ID 0 @Mid playMontion(%d,"352116@reset")
		;strvar x y
		; GET_PLAYER_MAPINFO 1 1 1 x y
		; effect 0 0 playEffct($@x$,$@y$,0,"effect\\entity\\352116.ent",15)
		
		;	get_task_var 4450 0 32 1 12
		
	strvar mid map
	get_role_var 45 32 1 mid 
	DEL_ID_MON @mid 1
		
	; add_sys_msg_start
	; add_sys_msg $@12$
	; set_sys_msg_mode 1 0 0
	; MSG_INFO
	PLAYER_CALL_BUB 302165 300 mid
	set_role_var 45 32 @mid 1

	; add_sys_msg_start
	; add_sys_msg ---$@12$
	; set_sys_msg_mode 1 0 0
	; MSG_INFO
;	if 12 = 0
	
		GET_PLAYER_MAPINFO 1 map 1 1 1 
	if map = 2000 or map = 2001 or map = 2002 or map = 2003 or map = 2005 or map = 2006 or map = 2007 or map = 2004
		
			rand 20 3
			if 20 = 0
				mon_speech @mid 10 2 1 哇，我刚才看到有一条河，你知道它流向哪里吗？
			endif
			if 20 = 1
				mon_speech @mid 10 2 1 我们会不会不小心掉到天空里？
			endif
			if 20 = 2
				mon_speech @mid 10 2 1 这个就是传说中的龙吗？它看起来一点都不凶！
			endif
			
	endif
	
	if map = 2100 or map = 2101 or map = 2102 or map = 2103 or map = 2105 or map = 2106 or map = 2107 or map = 2108
		;丘陵
		
		rand 20 3
		if 20 = 0
			mon_speech @mid 1 2 1 听说这里有很多野人，你见过野人吗？
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 听说野人会吃小孩子，是真的吗？
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 我们到船上去坐坐吧，顺便捕点鱼、虾什么的？
		endif
	endif
	
	if map = 2200
	;雪原
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 这里可真冷啊！
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 好多雪啊，时间城终年不下雪呢！
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 我们真应该穿厚点儿再来的，不过雪可真漂亮啊！
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 我们去拜访狼王和吸血鬼伯爵吧？他们会欢迎我们不？
		endif
	endif
	
	if map = 2300 
	;沙漠
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 你看这里遍地是黄金呢！
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 我们去暖泉镇看看吧，顺便泡泡温泉。
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 暖泉镇怎么会没有温泉呢？
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 阿努比斯就是那个长的很像狗的巨人吗？
		endif
	endif
	
	if map = 2400
	;落日
			
		rand 20 6
		if 20 = 0
			mon_speech @mid 1 2 1 这似乎跟我想象的不太一样，怎么说呢，有点荒凉。
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 你看那些草，都是枯黄的，秋天到了吗？
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 时间城一年到头都是春天，没有四季，没想到秋天是这样的！
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 这些土地下面真的有金子吗？我们挖开看看吧！
		endif
		if 20 = 4
			mon_speech @mid 1 2 1 落日荒野的夕阳，会不会比时间城还美呢？我们留下来看看吧！
		endif
		if 20 = 5
			mon_speech @mid 1 2 1 似乎深不见底呢，我们下去看看吧，你不好奇底下究竟有什么吗？
		endif
	endif
	
	if map = 2500
	;越雾
			rand 20 5
			if 20 = 0
				mon_speech @mid 1 2 1 这里的天气和时间城差不了多少。
			endif
			if 20 = 1
				mon_speech @mid 1 2 1 哇，好多动物！我们是不是可以当回狩猎者？
			endif
			if 20 = 2
				mon_speech @mid 1 2 1 你饿了吗？我们是不是先吃点什么呢？
			endif
			if 20 = 3
				mon_speech @mid 1 2 1 咦，我刚才似乎看到一个小个子…叔叔…
			endif
			if 20 = 4
				mon_speech @mid 1 2 1 这个世界真的有亡魂吗？那为什么我爸妈的亡魂不来看我呢？
			endif
	endif
	if map = 1000
		mon_speech @mid 1 2 1 有点失望呢，没有看到怪蜀黍，也没有看到大恶人。
	endif
	
	
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		task_stat 4450 1 20
		task_stat 4451 1 21
		task_stat 4452 1 22
		task_stat 4453 1 23
		task_stat 4454 1 24
		task_stat 4455 1 25
		task_stat 4456 1 26
		if 20 = 2
			
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 16414 16404 10
			if 2 = 2000 or 2 = 2001 or 2 = 2002 or 2 = 2003 or 2 = 2005 or 2 = 2006 or 2 = 2007 or 2 = 2004
				if 10 <= 500  
					task_fini 4450 1
				endif
			endif
		
		endif
		
		if 21 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 15861 9965 10
			if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108
				if 10 <= 500  
					task_fini 4451 1
				endif
			endif
		endif
		if 22 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 21995 22804 10
			if 2 = 2200
				if 10 <= 500
					task_fini 4452 1
				endif
			endif
		endif
		if 23 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 6707 22577 10
			if 2 = 2300
				if 10 <= 500
					task_fini 4453 1
				endif
			endif
		endif
		if 24 = 2
			;落日
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 21784 14086 10
			if 2 = 2400
				if 10 <= 500
					task_fini 4454 1
				endif
			endif
		endif
		if 25 = 2
			;越雾
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 10096 27570 10
			if 2 = 2500
				if 10 <= 500
					task_fini 4455 1
				endif
			endif
		endif
		if 26 = 2
			;时间城
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 31000 19000 10
			if 2 = 1000
				if 10 <= 500
					task_fini 4456 1
				endif
			endif
		
		endif
		
;	endif
endproc 

