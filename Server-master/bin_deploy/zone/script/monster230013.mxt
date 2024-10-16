;彩虹岛霸王龙


proc 1
	; add_sys_msg_start
	; add_sys_msg 寒风中传来了狼王刺耳的嚎啸！恐惧再次笼罩整个#L银风雪原@@(2200,2334,22181)#n！
	; set_sys_msg_mode 1 0 0
	; svr_msg_info
	
	strvar mid
	GET_MON_INFO 0 0 mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @mid 1095 1 25920000 0
endproc 
proc 2
	; add_sys_msg_start
	; add_sys_msg $_player$与他的伙伴最终战胜了狼王塔伯特！使银风雪原再次恢复以往的平静！
	; set_sys_msg_mode 1 0 0
	; svr_msg_info
	strvar x y 
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_LIFE_NPC 0 307034 @x @y 0 180 npc307034.mac
endproc 