;格瓦拉

proc 1
	DIALOG 1
	strvar Rflag Maxlv ret num ret1 num1
	GET_CUR_MAX_LEVEL Rflag Maxlv
	GET_ITEM_NUM_INPAK 5060080 ret num
	GET_VAILD_ITEM_NUM_INPAK 5060080 ret1 num1
	
	add_talk $_npc$:
	if Rflag = 0 and Maxlv <= 79
		add_talk #b#b近来失落之域的魔物变得异常躁动，为了找到背后的源头，誓卫军已深入失落之域底层。
		add_talk #b#b如今已过去一段时间，经验告诉我事情似乎并没想象的简单。希望你能突破各层失落之域，找到那里的誓卫军。
	endif
	if Rflag = 0 and Maxlv > 79
		add_talk #b#b我能感觉到一股强大的正从失落之域的底层肆意的涌出，也许修斯佩恩的封印已再次被解除。
		; add_talk #b#b事关重大，我将授予你【#c005ebe勇士之证#n】，希望你能突破各层失落之域，召集先锋者。
		; add_talk #b#b
		; add_talk #b #c005ebe进入失落之域七层，需要持有【勇士之证】
	endif
	add_talk #b#b
	strvar LV
	GET_ID_PLAYER_INFO 0 8 LV
	if LV < 70
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 达到70级后可向失落之域各层【誓卫军】领取经验#n
	else
		add_talk #c186506【勇闯失落之域】活动说明：
		add_talk 突破各层失落之域，向【誓卫军】领取经验
		add_talk 越高层数的誓卫军，可领取的经验越多#n
	
	; if Rflag = 0 and Maxlv > 69
		; if ret = 0 and num < 1
			; option 200 #请授予我【勇者之证】
		; endif
		; if ret = 0 and num > 0
			; if ret1 = 0 and num1 < 1
				; option 201 #换取新的【勇者之证】
			; endif
		; endif
	; endif
			option 300 【誓卫军】有哪些成员
	endif
	talk
endproc

proc 200
	strvar ret num
	GET_ITEM_NUM_INPAK 5060080 ret num
	
	if ret = 0 and num < 1
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060080 1 1 0
		
		add_sys_msg_start
		add_sys_msg 你获得了崭新的【勇者之证】，向第七层勇敢迈进吧！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 201
	strvar ret num
	GET_ITEM_NUM_INPAK 5060080 ret num

	if ret = 0 and num > 0
		DEC_ITEM_NUM_INPAK 5060080 @num 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060080 1 1 0
	
		add_sys_msg_start
		add_sys_msg 你换取了崭新的【勇者之证】，向第七层勇敢迈进吧！
		set_sys_msg_mode 1 0 0
		msg_info
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
	add_talk #b#b为了找到魔物躁动的原头，我已遣派誓卫军驻守在失落之域各层。
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
