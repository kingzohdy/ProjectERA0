;亚力山大

proc 1
	DIALOG 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	add_talk $_npc$:
	if Rflag = 0 and Maxlv > 79
		add_talk #b#b修斯佩恩的封印已彻底被解除，世界的格局就赌在了这场史诗之战！
	endif
	add_talk #b#b
	strvar ret LV
	GET_ID_PLAYER_INFO 0 8 LV
	PLOY_GC_GET_AWARD ret 7
	if LV < 70
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 达到70级后可向失落之域各层【誓卫军】领取经验#n
	else
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 突破各层失落之域，向【誓卫军】领取经验
		add_talk 越高层数的誓卫军，可领取的经验越多#n
		
		if ret != -1 and ret != -2
			option 200 #向“誓卫军团长”报到（领取经验）
		endif
		option 300 【誓卫军】有哪些成员
	endif
	
	talk
endproc

proc 200
	strvar ret Retm Con Val
	PLOY_GC_GET_AWARD ret 7
	
	if ret != -1 and ret != -2
		GET_ID_PLAYER_INFO 0 19 Con	
		if Con != -1
			var Val @Con
			dev Val 100
			mul Val 12
			; mul Val 10000
			; dev Val 10000
	
			ADD_EXP 0 @Val Retm
			if Retm = 0
				PLOY_GC_SET_AWARDE 0 7
				
				add_sys_msg_start
				add_sys_msg $_player$在【勇闯失落之域】活动中成功突破第七层，领取了大量经验！
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
