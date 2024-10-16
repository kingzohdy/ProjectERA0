
proc 1
	dialog 1
	add_talk $_npc$:
	
	is_in_my_clan_city ret
	if ret = 1
	;是本公会成员
		strvar evret
		IS_CLAN_SPY_OPEN evret
		if evret = 1
		; 活动开启了
			strvar lanjie
			IS_CLAN_CAN_SPY lanjie
			if lanjie = 0
			; 可以做偷取情报的任务
				add_talk #b#b找到目标公会的情报兵，从他们身上夺取情报。
				add_talk #b#b小心目标公会的成员，被他们击杀，情报将被抢夺回去。
				add_talk #b#b#c005ebe任务进展中，在目标公会城市击杀玩家不增加杀气值#n
				add_talk #b#b
				add_talk #c186506活动时间：周三、周五各1次(19:30-20:30)
				add_talk 活动进展：已开启
				add_talk 持续时间：活动持续60分钟
				add_talk 活动任务：情报夺取
				add_talk 战斗模式：建议切换成【公会模式】#n

				task_add_option 3681 1 100
				
				strvar ret2
				task_stat 3681 1 ret2
				if ret2 = 2
					option 170 #火速前往“目标公会”夺取情报
				endif
			elseif lanjie = 1
			; 可以做拦截情报的任务
				add_talk #b#b击杀那些夺取情报的公会成员，抢回公会情报。
				add_talk #b#b#c005ebe任务进展中，在己方公会城市击杀玩家不增加杀气值#n
				add_talk #b#b
				add_talk #c186506活动时间：周三、周五各1次(19:30-20:30)
				add_talk 活动进展：已开启
				add_talk 持续时间：活动持续60分钟
				add_talk 活动任务：情报拦截
				add_talk 战斗模式：建议切换成【公会模式】#n

				task_add_option 3682 1 101
			else
			endif
		elseif evret = 0
		;活动没开启
			add_talk #b#b知己知彼，方能百战不殆。
			strvar ret3 num ret4
			IS_WORLD_CITY_OWNER ret3
			player_clan_develop_ranking num ret4
			if ret3 = 0 and ret4 = 0 and num != 1
			; 非时间城占领公会和繁荣度最高公会
				add_talk #b#b最繁荣鼎盛或占领了时间城的公会，无疑是我们最大的竞争对手。为了与其抗衡，情报夺取成了重要的战略部署。
				add_talk #b#b等时机成熟时，我将开启【情报夺取】计划，需要大家积极配合行动。
				add_talk #b#b
				add_talk #c186506活动时间：周三、周五各1次(19:30-20:30)
				add_talk 活动开启：到时间自动开启
				add_talk 开启要求：世界至少6个公会登上公会排行榜
				add_talk 活动进展：未开启
				add_talk 持续时间：活动持续60分钟
				add_talk 活动任务：情报夺取#n
			elseif ret3 = 0 and ret4 = 0 and num = 1
			; 非时间城占领公会，是繁荣度最高公会
				add_talk #b#b作为最繁荣鼎盛的公会，我们无疑成了各公会最大的竞争对手。为了保护公会的战略机密，情报拦截成了重要的部署。
				add_talk #b#b发现夺取者时，我将开启【情报拦截】计划，希望大家积极配合行动。
				add_talk #b#b
				add_talk #c186506活动时间：周三、周五各1次(19:30-20:30)
				add_talk 活动开启：到时间自动开启
				add_talk 开启要求：世界至少6个公会登上公会排行榜
				add_talk 活动进展：未开启
				add_talk 持续时间：活动持续60分钟
				add_talk 活动任务：情报拦截#n
			elseif ret3 = 1
			; 是时间城占领公会
				add_talk #b#b作为时间城的占领者，我们无疑成了各公会最大的竞争对手。为了保护公会的战略机密，情报拦截成了重要的部署。
				add_talk #b#b发现夺取者时，我将开启【情报拦截】计划，希望大家积极配合行动。
				add_talk #b#b
				add_talk #c186506活动时间：周三、周五各1次(19:30-20:30)
				add_talk 活动开启：到时间自动开启
				add_talk 开启要求：世界至少6个公会登上公会排行榜
				add_talk 活动进展：未开启
				add_talk 持续时间：活动持续60分钟
				add_talk 活动任务：情报拦截#n
			else
			endif
		else
		endif		
	elseif ret = 0
	;不是本公会成员的npc对话
		add_talk #b#b知己知彼，方能百战不殆。
	else
	endif
	talk
endproc





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以下备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以下备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以下备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以下备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以下备份
proc 888
	dialog 1
	add_talk $_npc$:
	
	strvar ret ret1
	is_in_my_clan_city ret
	;是否本公会成员
	
	if ret = 1
		strvar evret
	;	GET_EVENT_STAT evret 48
		IS_CLAN_SPY_OPEN evret
		if evret = 1
			strvar pos pret
			strvar ret
			IS_CLAN_SPY_TASK_OPEN ret
			IS_CLAN_ANTI_SPY_TASK_OPEN ret1
			if ret = 1 and ret1 = 0
				add_talk #b#b情报夺取任务中:您要前去的是实力最为强大的公会城市,夺取情报是非常困难的,建议和公会的战友们选择相同的入侵地点.也可以联合其他公会一起前往.	
				add_talk #b#b注意！在目标公会城市内，是可以自由PK，击杀其他玩家是不会增加杀气值的，建议您和您的战友们，开启公会模式，也可以和其他前往夺取情报的公会进行合作！	
			endif
			if ret = 0 and ret1 = 1
				add_talk #b#b情报防守任务中:拦截其他公会的人前来偷取情报,也会面临重重困难,因为您将面对几倍于您的敌人,但是,勇敢的战斗吧,哪怕以伤换伤,记住,每击杀一个敌人,都会给您给来奖励,而死亡的惩罚微乎其微!
				add_talk #b#b注意！在目标公会城市内，是可以自由PK，击杀其他玩家是不会增加杀气值的，建议您和您的战友们，开启公会模式，也可以和其他前往夺取情报的公会进行合作！
				endif
			if ret = 1 and ret1 = 1
			;都开启
				add_talk #b#b情报夺取任务中:您要前去的是实力最为强大的公会城市,夺取情报是非常困难的,建议和公会的战友们选择相同的入侵地点.也可以联合其他公会一起前往.
				add_talk #b#b情报防守任务中:拦截其他公会的人前来偷取情报,也会面临重重困难,因为您将面对几倍于您的敌人,但是,勇敢的战斗吧,哪怕以伤换伤,记住,每击杀一个敌人,都会给您给来奖励,而死亡的惩罚微乎其微!
				add_talk #b#b注意！在目标公会城市内，是可以自由PK，击杀其他玩家是不会增加杀气值的，建议您和您的战友们，开启公会模式，也可以和其他前往夺取情报的公会进行合作！
				endif
			if ret = 0 and ret1 = 0
			;都没开启
				add_talk #b#b知己知彼,方能百战不殆,情报的重要性毋庸置疑,夺取竞争对手公会的情报,不仅能了解对手,同时也能打击对手.
				add_talk #b#b当公会繁荣度达到全服第一,并且时间城没有被占领的时候,对于其他公会来说,争夺时间城时,我们将会是最大的威胁,更需要防备对手前来夺取情报,这一点至关重要.如果我们公会占领了时间城,那么更将成为众矢之的,所以,守护好自己公会的情报比探取别人的情报更为重要!
				add_talk #b#b在活动期间,需要有职务的公会官员开启任务,请各位公会官员注意活动时间:每周二和周四,晚上19:30-20:30.
				add_talk #b#b活动期间,占领时间城的公会(如果时间城无主状态,那么繁荣度第一的公会)自动开启情报拦截任务,其他公会自动开启情报夺取任务.任务难度非同一般,叫上你的战友,一起出发吧.

			endif
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
			strvar lanjie
			IS_CLAN_CAN_SPY lanjie
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			if lanjie = 0
			;不可被刺探，只能做夺取
				if ret = 0
				;刺探未开启
					option 150 #开启情报夺取活动
				endif
			endif
			task_stat 3682 1 40
			if ret = 1 and 40 = 0
			;刺探已开启 并且未接取防守
				task_add_option 3681 1 100
			endif
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			if lanjie = 1
			;可被刺探
				if ret1 = 0
				;防守未开启
					option 160 #开启情报拦截活动
				endif
			endif
			task_stat 3681 1 41
			if ret1 = 1 and 41 = 0
			;防守任务已开启 并且未接取刺探
				task_add_option 3682 1 101
			endif
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			;刺探已开启
			strvar ret2
			task_stat 3681 1 ret2
			if ret2 = 2
				option 170 #前往目标公会夺取情报
			endif
		else
			add_talk #b#b知己知彼,方能百战不殆,情报的重要性毋庸置疑,夺取竞争对手公会的情报,不仅能了解对手,同时也能打击对手.
			add_talk #b#b当公会繁荣度达到全服第一,并且时间城没有被占领的时候,对于其他公会来说,争夺时间城时,我们将会是最大的威胁,更需要防备对手前来夺取情报,这一点至关重要.如果我们公会占领了时间城,那么更将成为众矢之的,所以,守护好自己公会的情报比探取别人的情报更为重要!
			add_talk #b#b在活动期间,需要有职务的公会官员开启任务,请各位公会官员注意活动时间:每周二和周四,晚上19:30-20:30.
			add_talk #b#b活动期间,占领时间城的公会(如果时间城无主状态,那么繁荣度第一的公会)自动开启情报拦截任务,其他公会自动开启情报夺取任务.任务难度非同一般,叫上你的战友,一起出发吧.	
		endif
	else
		add_talk #b#b知己知彼,方能百战不殆,情报的重要性毋庸置疑,夺取竞争对手公会的情报,不仅能了解对手,同时也能打击对手.
		add_talk #b#b当公会繁荣度达到全服第一,并且时间城没有被占领的时候,对于其他公会来说,争夺时间城时,我们将会是最大的威胁,更需要防备对手前来夺取情报,这一点至关重要.如果我们公会占领了时间城,那么更将成为众矢之的,所以,守护好自己公会的情报比探取别人的情报更为重要!
		add_talk #b#b在活动期间,需要有职务的公会官员开启任务,请各位公会官员注意活动时间:每周二和周四,晚上19:30-20:30.
		add_talk #b#b活动期间,占领时间城的公会(如果时间城无主状态,那么繁荣度第一的公会)自动开启情报拦截任务,其他公会自动开启情报夺取任务.任务难度非同一般,叫上你的战友,一起出发吧.
	endif
	talk

endproc 
proc 150
	PLAYER_CLAN_POS_INFO pret 0 0 0 pos
	if pret = 0 and pos != 0
	
	else
;		add_sys_msg_start
;		add_sys_msg 只有公会官员有权限开启活动
;		set_sys_msg_mode 4 0 0
;		msg_info
		return
	endif
	IS_CLAN_SPY_TASK_OPEN ret
	if ret = 0
		CLAN_SPY_TASK_OPEN 1
		strvar
		GET_ID_PLAYER_INFO 0 18 clanID
		add_sys_msg_start
		add_sys_msg 您的公会已经开启情报夺取活动, 可以前往公会城市情报官处接受情报夺取任务,丰厚的奖励等待着你！
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @clanID
	endif
endproc
proc 160

	PLAYER_CLAN_POS_INFO pret 0 0 0 pos
	if pret = 0 and pos != 0
		IS_CLAN_CAN_SPY 60
		if 60 = 0
		;	add_sys_msg_start
		;	add_sys_msg 您的公会繁荣度不在排行榜前三位,无法开启情报拦截活动!
		;	set_sys_msg_mode 4 0 0
		;	msg_info
			return
		endif
	else
		add_sys_msg_start
		add_sys_msg 只有公会成员有权限开启活动
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	IS_CLAN_ANTI_SPY_TASK_OPEN ret1
	if ret1 = 0
		CLAN_ANTI_SPY_TASK_OPEN 1
		strvar
		GET_ID_PLAYER_INFO 0 18 clanID
		add_sys_msg_start
		add_sys_msg 您的公会已经开启情报拦截活动,可以前往公会城市情报官处接受情报防守任务,丰厚的奖励在等待着你！
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @clanID	
	endif
	
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上备份
proc 100
	task_do_option 3681 1
endproc 
proc 101
	task_do_option 3682 1
endproc 
proc 170
	open_ui 68
endproc 
proc 2
	get_trig_task 1 2
	strvar clanID
	GET_ID_PLAYER_INFO 0 18 clanID
	if 2 = 3681 
	;刺探
		GET_ITEM_NUM_INPAK 5300950 1 31
		DEC_ITEM_NUM_INPAK 5300950 @31 1
		GET_ITEM_NUM_INPAK 5300953 1 32
		DEC_ITEM_NUM_INPAK 5300953 @32 1
		GET_ITEM_NUM_INPAK 5300952 1 33
		DEC_ITEM_NUM_INPAK 5300952 @33 1
		
		open_ui 68
		
	;	add_sys_msg_start
	;	add_sys_msg 公会副会长【$_player$】开启了寻找公会宝藏活动！
	;	set_sys_msg_mode 14 0 0
	;	CLAN_MSG_INFO 1 @clanID
	endif
	if 2 = 3682
	;防守
		add_sys_msg_start
		add_sys_msg 公会成员$_player$,接受了防守任务,誓要将情报夺取者驱逐出境!
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @clanID
	endif
endproc 





