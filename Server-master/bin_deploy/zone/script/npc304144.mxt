;拿破仑

proc 1
	DIALOG 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	add_talk $_npc$:
	if Rflag = 0 and Maxlv <= 79
		add_talk #b#b这里竟然还能见到活人，哈哈哈！
		add_talk #b#b#name#你听说过修斯佩恩吗，这个深渊领主就封印在失落之域的最底层。
		add_talk #b#b至于魔物躁动的原因，希望不是最坏的情况。
	endif
	if Rflag = 0 and Maxlv > 79
		add_talk #b#b魔物已变得难以抵挡！请你火速前往第七层！这里交给我们誓卫军殿后！
		add_talk #b#b世界就在你的手中！
	endif
	add_talk #b#b
	strvar ret LV
	GET_ID_PLAYER_INFO 0 8 LV
	PLOY_GC_GET_AWARD ret 5
	if LV < 70
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 达到70级后可向失落之域各层【誓卫军】领取经验#n
	else
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 突破各层失落之域，向【誓卫军】领取经验
		add_talk 越高层数的誓卫军，可领取的经验越多#n
		
		if ret != -1 and ret != -2
			option 200 #向“誓卫军兵长”报到（领取经验）
		endif
			option 300 【誓卫军】有哪些成员
	endif
		option 400 关于世界领主【修斯佩恩】
	
	talk
endproc

proc 200
	strvar ret Retm Con Val
	PLOY_GC_GET_AWARD ret 5
	
	if ret != -1 and ret != -2
		GET_ID_PLAYER_INFO 0 19 Con	
		if Con != -1
			var Val @Con
			dev Val 100
			mul Val 11
			; mul Val 10000
			; dev Val 10000
	
			ADD_EXP 0 @Val Retm
			if Retm = 0
				PLOY_GC_SET_AWARDE 0 5
				
				add_sys_msg_start
				add_sys_msg $_player$在【勇闯失落之域】活动中成功突破第五层，领取了大量经验!
				set_sys_msg_mode 3 0 0
				SVR_MSG_INFO
			endif
		endif
	endif
endproc

proc 300
	DIALOG 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	strvar ret1 ret2 ret3 ret4 ret5
	PLOY_GC_GET_AWARD ret1 1
	PLOY_GC_GET_AWARD ret2 2
	PLOY_GC_GET_AWARD ret3 4
	PLOY_GC_GET_AWARD ret4 5
	PLOY_GC_GET_AWARD ret5 7
	
	
	add_talk $_npc$:
	add_talk #b#b我们誓卫军先锋者分布失落之域各层，各自驻守着相应的区域。
	add_talk #b#b
	add_talk #c186506【勇闯失落之域】活动说明：
	add_talk 突破各层失落之域，向【誓卫军】领取经验
	add_talk 越高层数的誓卫军，可领取的经验越多#n
	add_talk #b#b
	if ret1 = -2
		add_talk 第一层：誓卫军司长（#c186506已领取经验#n）
	else
		add_talk 第一层：#L誓卫军司长@@(3211,23400,21900)#n（未领取经验）
	endif
	if ret2 = -2
		add_talk 第二层：誓卫军班长（#c186506已领取经验#n）
	else
		add_talk 第二层：#L誓卫军班长@@(3212,19900,22800)#n（未领取经验）
	endif
	if ret3 = -2
		add_talk 第四层：誓卫军队长（#c186506已领取经验#n)
	else
		add_talk 第四层：#L誓卫军队长@@(3214,13800,16800)#n（未领取经验）
	endif
	
	if Rflag = 0 and Maxlv > 74
		if ret4 = -2
			add_talk 第五层：誓卫军兵长（#c186506已领取经验#n）
		else
			add_talk 第五层：#L誓卫军兵长@@(3215,26500,6000)#n（未领取经验）
		endif
	endif
	
	if Rflag = 0 and Maxlv > 79
		if ret5 = -2
			add_talk 第七层：誓卫军团长（#c186506已领取经验#n）
		else
			add_talk 第七层：#L誓卫军团长@@(3217,9300,6400)#n（未领取经验）
		endif
	endif
	
	
	option 1 返回
	talk
endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b关于深渊领主【修斯佩恩】的传说我想大家都不陌生。
	add_talk #b#b曾经世界几乎毁灭在这头上古魔兽手上，以人类当时的力量根本无法将其消灭，最后人类只能付出极大的代价将其封印。
	add_talk #b#b然而这一切并非传说，修斯佩恩就封印在了这失落之域第七层。
	add_talk #b#b而我们誓卫军的的使命则是守护这个封印，直到灾难再次降临那天。
	
	
	option 1 返回
	talk
endproc
