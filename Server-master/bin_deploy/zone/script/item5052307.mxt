;劳动芳华

proc 1
	GET_PACK_SPACE 0 20
	if 20 < 1
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	rand 10 10000
	if 10 < 5099
		call 100
	elseif 10 < 5199
		call 101
	elseif 10 < 6199
		call 102
	elseif 10 < 7199
		call 110
	elseif 10 < 7299
		call 120
	elseif 10 < 7499
		call 130		
	elseif 10 < 9999
		call 160
	else
		call 170
	endif
endproc

proc 100
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_BIND_ITEM_NUM_INPAK 5052899 2 1 13
	endif
endproc

proc 101
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 3651003 1 1 13
		add_sys_msg_start
		add_sys_msg $_player$轻轻松松从劳模奖品礼盒中抽到【重弦(7天)背饰】，手气真心不可思议！
		set_sys_msg_mode 18 0 0
		SVR_MSG_INFO
	endif
endproc

proc 102
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_BIND_ITEM_NUM_INPAK 5052304 10 1 13
	endif
endproc

proc 110
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_BIND_ITEM_NUM_INPAK 5052310 1 1 13
	endif
endproc

proc 120
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 5043248 1 1 13
	endif
endproc

proc 130
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 5043249 1 1 13
	endif
endproc

;奖励锄禾
proc 140
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 5043250 1 1 13
	endif
endproc

;奖励当午
proc 150
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 5043251 1 1 13
	endif
endproc

proc 160
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		GET_ID_PLAYER_INFO 0 8 10
		strvar itemId
		if 10 < 50
			var itemId 5052316
		elseif 10 < 60
			var itemId 5052317
		elseif 10 < 70
			var itemId 5052318
		elseif 10 < 80
			var itemId 5052319
		else
			var itemId 5052328
		endif
		ADD_BIND_ITEM_NUM_INPAK @itemId 2 1 0
	endif
endproc

proc 170
	DEC_ITEM_NUM_INPAK 5052307 1 11
	if 11 = 0
		ADD_ITEM_NUM_INPAK 5600219 1 1 13
		add_sys_msg_start
		add_sys_msg $_player$轻轻松松从劳模奖品礼盒中抽到【古铠刺尾龙（稀有坐骑）】，手气真心不可思议！
		set_sys_msg_mode 18 0 0
		SVR_MSG_INFO
	endif
endproc
