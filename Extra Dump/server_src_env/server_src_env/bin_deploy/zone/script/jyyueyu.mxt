
;探监

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b看过<<越狱>>没?相当精彩啊,看了几集,咱也可以接一接越狱的生意了,嘿嘿~,技术当然没越狱里的主角牛逼,所以呢,每天最多帮你三次,每次难度都会递增的,所以呢收费也是不一样的,第一次1000银币,第二次需要5000银币,第三次需要20000银币!

	;五一活动
	task_stat 4439 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
		get_yueyu_num 10 20
		if 10 = 0 and 20 = 0
			option 150 #我要越狱
		endif
		if 10 = 0 and 20 = 1
			option 160 #我要越狱
		endif
		if 10 = 0 and 20 = 2
			option 170 #我要越狱
		endif

	talk
	
endproc 

proc 150
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #返回值 #yes事件号 #no事件号 #提示信息
		choose 1 151 152 今天第1次越狱,需要支付1000银币,是否确认?
	else
		add_sys_msg_start
		add_sys_msg 您的杀气值不足200，可以自由离开监狱,不需要越狱了!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 152
	return
endproc
proc 151
	get_player_id 1 30
	DEC_MONEY_TEST @30 1000 20 40
	
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 1000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 银币不足！
		set_sys_msg_mode 4 0 0
		MSG_INFO 
	endif
endproc 

proc 160
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #返回值 #yes事件号 #no事件号 #提示信息
		choose 1 161 152 今天第2次越狱,需要支付5000银币,是否确认?
	else
		add_sys_msg_start
		add_sys_msg 您的杀气值不足200，可以自由离开监狱,不需要越狱了!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 161
	get_player_id 1 30
	DEC_MONEY_TEST @30 5000 20 40
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 5000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 银币不足！
		set_sys_msg_mode 4 0 0
		MSG_INFO 
	endif
endproc
proc 170
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #返回值 #yes事件号 #no事件号 #提示信息
		choose 1 171 152 今天第3次越狱,需要支付20000银币,是否确认?
	else
		add_sys_msg_start
		add_sys_msg 您的杀气值不足200，可以自由离开监狱,不需要越狱了!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 171
	get_player_id 1 30
	DEC_MONEY_TEST @30 20000 20 40
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 20000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 银币不足！
		set_sys_msg_mode 4 0 0
		MSG_INFO 
	endif
endproc

;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4439 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b他们都说我是疯子，我看他们才真正疯了！愚昧的人类！我是天才，那群傻瓜怎么会理解天才的寂寞，你愿意留下来和我一起玩游戏吗？
			option 511 #下次……下次一定……
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“慰问物资”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc