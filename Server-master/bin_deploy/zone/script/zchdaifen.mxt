	
proc 1
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b您好！亲爱的玩家，这里将提供您纪念币兑换的活动，您可以随意的兑换需要的纪念币，不过我们会回收些许货币作为回报哦！
	
	strvar ItmNum32 ItmNum33 ItmNum34

	var ItmNum32 0
	var ItmNum33 0
	var ItmNum34 0
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum32
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum33
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum34
	
;	add_talk “零”：#c186506“$@ItmNum32$”#n个
;	add_talk “纪”：#c186506“$@ItmNum33$”#n个
;	add_talk “元”：#c186506“$@ItmNum34$”#n个	
;	
;	 option 200 使用三个“零”兑换两个“纪”
;	 option 201 使用三个“零”兑换两个“元”
;	 option 202 使用三个“纪”兑换两个“零”
;	 option 203 使用三个“纪”兑换两个“元”
;	 option 204 使用三个“元”兑换两个“零”
;	 option 205 使用三个“元”兑换两个“纪”
	talk
endproc 
proc 300
	return
endproc

proc 200
	CHOOSE 1 250 300 确定使用三个“零”兑换两个“纪”吗？
endproc 
proc 250
	strvar ItmNum3 Num ItmNum4 Reti

	GET_ITEM_NUM_INPAK 5060017 1 ItmNum3
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum4
	if ItmNum3 > 1 
		GET_PACK_SPACE 0 Num 		
;		ADD_ITEM_NUM_INPAK 5060018 2 2 Reti
;		if Reti = -2
		if ItmNum4 = 0 and Num = 0 
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
		
		DEC_ITEM_NUM_INPAK 5060017 2 1
		ADD_ITEM_NUM_INPAK 5060018 2 0 0
		
		add_sys_msg_start
		add_sys_msg 恭喜你”通过三个“零”兑换获得两个“纪”!
		set_sys_msg_mode 3 0 0
		msg_info
	
	else
		add_sys_msg_start
		add_sys_msg 非常抱歉，背包中的“零”不足以兑换！
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc
