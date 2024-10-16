proc 1


	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b世界仿佛风平浪静的海面，在海底蕴藏着数不尽的未知威胁，一点小差错足以致命。
	add_talk #b#b我在此的目的就是向实力足够的勇士委以副本级别的重任。
	add_talk #b#b
	add_talk #c186506副本任务[50级]-人物达到50级后可在此接取副本级别的任务，完成后将获得稀有奖励

	task_add_option 3027 1 100
	; task_stat 3027 1 10
	task_add_option 3028 1 200
	; task_stat 3028 1 10
	task_add_option 3029 1 300
	; task_stat 3029 1 10
	task_add_option 3030 1 400
	; task_stat 3030 1 10
	task_add_option 3031 1 500
	; task_stat 3031 1 10
	
	strvar num num1 num2 num3 ret
	GET_ITEM_NUM_INPAK 5062200 0 num
	GET_ITEM_NUM_INPAK 5062201 0 num1
	GET_ITEM_NUM_INPAK 5062202 0 num2
	GET_ITEM_NUM_INPAK 5062203 0 num3
	if num > 0 or num1 > 0 or num2 > 0 or num3 > 0
		OPTION 600 #我这有些消失的武器
	else
		OPTION 605 #你知道消失的武器吗
	endif

	talk
endproc 

proc 100
	task_do_option 3027 1
endproc 

proc 200
	task_do_option 3028 1
endproc

proc 300
	task_do_option 3029 1
endproc

proc 400
	task_do_option 3030 1
endproc

proc 500
	task_do_option 3031 1
endproc

proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<米修斯沉重的凝视着接过来的武器>#n
	add_talk #b#b这是守护着失落之域的先辈们遗落的武器。
	add_talk #b#b我这有些许银币，希望你能把这武器让给我。
		
	OPTION 601 交出“消失的宝剑”，兑换150银币
	OPTION 602 交出“消失的剑刃”，兑换30银币
	OPTION 603 交出“消失的剑柄”，兑换20银币
	OPTION 604 交出“消失的剑鞘”，兑换20银币
	
	talk
endproc

proc 601
	strvar num ret
	GET_ITEM_NUM_INPAK 5062200 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062200 1 ret
		if ret = 0
			ADD_MONEY 0 150 20 1
			
			add_sys_msg_start
			add_sys_msg 你接过米修斯的银币，交出了“消失的宝剑”
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg “消失的宝剑”已被损坏，暂时无法兑换银币
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包里找不到“消失的宝剑”，暂时无法兑换银币
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 602
	strvar num ret
	GET_ITEM_NUM_INPAK 5062201 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062201 1 ret
		if ret = 0
			ADD_MONEY 0 30 20 1
			
			add_sys_msg_start
			add_sys_msg 你接过米修斯的银币，交出了“消失的剑刃”
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg “消失的剑刃”已被损坏，暂时无法兑换银币
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包里找不到“消失的剑刃”，暂时无法兑换银币
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 603
	strvar num ret
	GET_ITEM_NUM_INPAK 5062202 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062202 1 ret
		if ret = 0
			ADD_MONEY 0 20 20 1
			
			add_sys_msg_start
			add_sys_msg 你接过米修斯的银币，交出了“消失的剑柄”
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg “消失的剑柄”已被损坏，暂时无法兑换银币
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包里找不到“消失的剑柄”，暂时无法兑换银币
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 604
	strvar num ret
	GET_ITEM_NUM_INPAK 5062203 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062203 1 ret
		if ret = 0
			ADD_MONEY 0 20 20 1
			
			add_sys_msg_start
			add_sys_msg 你接过米修斯的银币，交出了“消失的剑鞘”
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg “消失的剑鞘”已被损坏，暂时无法兑换银币
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包里找不到“消失的剑鞘”，暂时无法兑换银币
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 605
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b传说大量魔物曾向失落之域疯狂的屠杀。为了守护身后无数的生命，勇者们坚守在死亡前线。
	add_talk #b#b但遗憾的是勇者最终全军覆没，只留下这些被遗忘的武器。
	add_talk #b#b#c186506如果你有幸找到这些消失的武器，我愿意用银币向你们收下，勇者的英灵应得到更庄严的对待。#n
	add_talk #b#b
	add_talk #c005ebe获取途径：
	add_talk #b#b失落之域的怪物有几率掉落“失落的剑刃”“失落的剑柄”“失落的剑鞘”#n
	add_talk #c6d00d2武器合成：
	add_talk #b#b“失落的剑刃”“失落的剑柄”“失落的剑鞘”可合成“失落的宝剑”#n
	
	OPTION 1 #返回

	talk
endproc
