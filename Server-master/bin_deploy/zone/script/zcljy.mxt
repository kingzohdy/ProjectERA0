proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b亲爱的玩家您好！我是零纪元活动特派大使！
	add_talk #b#b为了感谢大家对我们零纪元的厚爱与支持，只要找到“我”“爱”“零”“纪”“元”字眼的纪念币，便能向我领取丰厚的礼物哦！
	add_talk #b#b#c186506当然，您可以从各种怪物身上或副本中找到它们！亲！#n
	add_talk #b#b出发吧！勇敢的冒险家！
;	add_talk 已找到：
	
;	strvar ItmNum10 ItmNum11 ItmNum12 ItmNum13 ItmNum14
;	var ItmNum10 0
;	var ItmNum11 0
;	var ItmNum12 0
;	var ItmNum13 0
;	var ItmNum14 0
;	GET_ITEM_NUM_INPAK 5060015 1 ItmNum10
;	GET_ITEM_NUM_INPAK 5060016 1 ItmNum11
;	GET_ITEM_NUM_INPAK 5060017 1 ItmNum12
;	GET_ITEM_NUM_INPAK 5060018 1 ItmNum13
;	GET_ITEM_NUM_INPAK 5060019 1 ItmNum14
;	
;	add_talk “我”：#c186506“$@ItmNum10$”#n个
;	add_talk “爱”：#c186506“$@ItmNum11$”#n个
;	add_talk “零”：#c186506“$@ItmNum12$”#n个
;	add_talk “纪”：#c186506“$@ItmNum13$”#n个
;	add_talk “元”：#c186506“$@ItmNum14$”#n个
	
	task_add_option 3120 1 101	
	
	option 150 #使用单个纪念币兑换列表
	option 210 #使用“零”“纪”“元”兑换列表
	option 220 #使用“我”“爱”“零”“纪”“元”兑换列表
;	option 500 媒体兑奖
			  
	
	talk
endproc 
proc 101
    task_do_option 3120 1
endproc
proc 150
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#b您好，请问您需要兑换什么礼物吗？
	; add_talk #b#b
	add_talk 开启【#c005ebe经典宝箱#n】将有机会获得下列珍宝！：
	add_talk #b#b#c6d00d2幽灵蝠鲨#n——稀有坐骑！
	add_talk #b#b#c6d00d2小蓝龙#n——稀有宠物！
	
	add_talk #b#b
	; add_talk 已找到：
	
	strvar ItmNum10 ItmNum11 ItmNum12 ItmNum13 ItmNum14
	var ItmNum10 0
	var ItmNum11 0
	var ItmNum12 0
	var ItmNum13 0
	var ItmNum14 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum10
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum11
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum12
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum13
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum14
	
	add_talk “我”：已找到#c186506$@ItmNum10$#n个
	add_talk “爱”：已找到#c186506$@ItmNum11$#n个
	add_talk “零”：已找到#c186506$@ItmNum12$#n个
	add_talk “纪”：已找到#c186506$@ItmNum13$#n个
	add_talk “元”：已找到#c186506$@ItmNum14$#n个
	
	option 200 #使用一个“我”兑换一个“经典宝箱”
	option 201 #使用一个“爱”兑换一个“经典宝箱”
	option 202 #使用三个“零”兑换三个“星辉宝钻”
	option 203 #使用三个“纪”兑换六个“宝石碎片”
	option 204 #使用三个“元”兑换两个“幻化果实”

	talk
endproc

proc 200
;我
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum
	if ItmNum < 1
		add_sys_msg_start
		add_sys_msg 你身上没有此物品！
		set_sys_msg_mode 1 0 0
		msg_info
	else
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060015 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052806 1 1 0
		else
			add_sys_msg_start
			add_sys_msg 请预留1个以上背包空格再开启！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
	call 150
endproc
proc 201
;爱
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum
	if ItmNum < 1
		add_sys_msg_start
		add_sys_msg 你身上没有此物品！
		set_sys_msg_mode 1 0 0
		msg_info
	else
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060016 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052806 1 1 0
		else
			add_sys_msg_start
			add_sys_msg 请预留1个以上背包空格再开启！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
	call 150
endproc 
proc 202
;零
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 2 Num
		
		if Num = -1 
			add_sys_msg_start
			add_sys_msg 背包没有足够的空间！
			set_sys_msg_mode 1 0 0
			msg_info
			
		else
		
			DEC_ITEM_NUM_INPAK 5060017 3 10
			if 10 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 1 0		
		
		endif

	endif
	call 150
endproc
proc 203
;纪
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 2 Num
	
		if Num = -1 
			add_sys_msg_start
			add_sys_msg 背包没有足够的空间！
			set_sys_msg_mode 1 0 0
			msg_info
		
		else
			DEC_ITEM_NUM_INPAK 5060018 3 11
			if 11 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 1 0
		endif
	endif
	call 150
endproc 
proc 204
;元
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 2 2 Num
	
		if Num = -1
			add_sys_msg_start
			add_sys_msg 背包没有足够的空间！
			set_sys_msg_mode 1 0 0
			msg_info
		else
		
			DEC_ITEM_NUM_INPAK 5060019 3 12
			if 12 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 2 1 0
		endif
	endif
	call 150
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 210
;零纪元
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#b只要找到#c186506“零”“纪”“元”#n就可以兑换例典宝箱哦！亲！
	; add_talk #b#b
	add_talk 开启【#c005ebe例典宝箱#n】将有机会获得下列珍宝！：
	add_talk #b#b#c6d00d2“我”#n——可用来兑换【#c005ebe经典宝箱#n】！
	add_talk #b#b#c6d00d2“爱”#n——可用来兑换【#c005ebe经典宝箱#n】！

	add_talk #b#b
	; add_talk 已找到：
	
	strvar ItmNum20 ItmNum21 ItmNum22 ItmNum23 ItmNum24
	var ItmNum20 0
	var ItmNum21 0
	var ItmNum22 0
	var ItmNum23 0
	var ItmNum24 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum20
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum21
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum22
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum23
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum24
	
	add_talk “我”：已找到#c186506$@ItmNum20$#n个
	add_talk “爱”：已找到#c186506$@ItmNum21$#n个
	add_talk “零”：已找到#c186506$@ItmNum22$#n个
	add_talk “纪”：已找到#c186506$@ItmNum23$#n个
	add_talk “元”：已找到#c186506$@ItmNum24$#n个
	
	option 211 #兑换一个“例典宝箱”
	
	talk

endproc 
proc 211
	strvar ItmNum1 ItmNum2 ItmNum3 Num
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum1
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum2
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum3
	if ItmNum1 > 0 and ItmNum2 > 0 and ItmNum3 > 0
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060017 1 1
			DEC_ITEM_NUM_INPAK 5060018 1 1
			DEC_ITEM_NUM_INPAK 5060019 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052805 1 1 0
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”通过“零纪元”活动兑换获得一个“例典宝箱”
			set_sys_msg_mode 18 0 0
			svr_msg_info
		else
			add_sys_msg_start
			add_sys_msg 请预留1个以上背包空格再开启！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
	call 210
endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc 220
;我爱
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#b只要找到#c186506“我”“爱”“零”“纪”“元”#n就可以兑换特典宝箱哦！亲！
	; add_talk #b#b
	add_talk 开启【#c005ebe特典宝箱#n】将有机会获得下列珍宝！：
	add_talk #b#b#c6d00d2骑士之魂#n——稀有坐骑！
	add_talk #b#b#c6d00d2公爵#n——稀有宠物！

	add_talk #b#b
	; add_talk 已找到：
	
	strvar ItmNum30 ItmNum31 ItmNum32 ItmNum33 ItmNum34
	var ItmNum30 0
	var ItmNum31 0
	var ItmNum32 0
	var ItmNum33 0
	var ItmNum34 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum30
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum31
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum32
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum33
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum34
	
	add_talk “我”：已找到#c186506$@ItmNum30$#n个
	add_talk “爱”：已找到#c186506$@ItmNum31$#n个
	add_talk “零”：已找到#c186506$@ItmNum32$#n个
	add_talk “纪”：已找到#c186506$@ItmNum33$#n个
	add_talk “元”：已找到#c186506$@ItmNum34$#n个	
	
	option 221 #兑换一个“特典宝箱”

	
	talk
endproc

proc 221
	strvar ItmNum1 ItmNum2 ItmNum3 ItmNum4 ItmNum5 Num
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum1
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum2
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum3
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum4
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum5
	if ItmNum1 > 0 and ItmNum2 > 0 and ItmNum3 > 0 and ItmNum4 > 0 and ItmNum5 > 0
		GET_PACK_SPACE 0 Num 
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060015 1 1
			DEC_ITEM_NUM_INPAK 5060016 1 1
			DEC_ITEM_NUM_INPAK 5060017 1 1
			DEC_ITEM_NUM_INPAK 5060018 1 1
			DEC_ITEM_NUM_INPAK 5060019 1 1
			
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052807 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”通过“我爱零纪元”活动兑换获得一个“特典宝箱”
			set_sys_msg_mode 18 0 0
			svr_msg_info
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
	call 220
endproc


proc 500
	dialog 1
	add_talk $_npc$:
	add_talk #b#b亲爱的玩家您好！我是艾美莉
	add_talk #b#b为了答谢大家对《零纪元》的厚爱和支持，您可以用您所收集到的神码包中的6种背饰：吐纸猫、小章鱼色色、小浣熊、绅士企鹅、小浪人、疯狂小易与我兑换“黄金礼包”哦！不但有神骑-拓拓还有更多的宝物等您来兑换，赶快行动吧！
	add_talk #b#b您还可以收集6张“抽奖券”来跟我抽取更多、更好的礼物哦！

		option 510 #我要用6种背饰兑换黄金礼包
		option 520 #我已收齐6种抽奖券，马上进行兑换抽奖

	talk
endproc
proc 510
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 3650003 1 it1
	GET_ITEM_NUM_INPAK 3653004 1 it2
	GET_ITEM_NUM_INPAK 3653003 1 it3
	GET_ITEM_NUM_INPAK 3653001 1 it4
	GET_ITEM_NUM_INPAK 3653000 1 it5
	GET_ITEM_NUM_INPAK 3653002 1 it6
	
	if it1 > 0 and it2 > 0 and it3 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 3650003 1 1
			DEC_ITEM_NUM_INPAK 3653004 1 1
			DEC_ITEM_NUM_INPAK 3653003 1 1
			DEC_ITEM_NUM_INPAK 3653001 1 1
			DEC_ITEM_NUM_INPAK 3653000 1 1
			DEC_ITEM_NUM_INPAK 3653002 1 1
			
			ADD_BIND_ITEM_NUM_INPAK 5052910 1 1 0                   
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 520
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054001 1 it2
	GET_ITEM_NUM_INPAK 5054002 1 it3
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it2 > 0 and it3 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 







