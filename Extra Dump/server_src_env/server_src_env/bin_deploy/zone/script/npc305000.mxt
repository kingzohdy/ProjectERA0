; 波尔


proc 1
	DIALOG 1
	add_talk #b#b#c6d00d2“同一个节拍！Juntos num sóritmo！”
	add_talk #b#b#b#b“支持你心目中的胜利之队”#n
	add_talk #b#b
	add_talk #b#b#cFF0080只要支持的球队晋级，即可获得丰厚的金券#n
	
		option 100 A组-国家队
		option 110 B组-国家队
		option 120 C组-国家队
		option 130 D组-国家队
		option 140 E组-国家队
		option 150 F组-国家队
		option 160 G组-国家队
		option 170 H组-国家队
	
	talk
endproc

proc 100
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 0 16 ret zhi1
	get_global_var 2 16 ret zhi2
	get_global_var 4 16 ret zhi3
	get_global_var 6 16 ret zhi4
	call 200
	
		option 101 支持-巴西队【已有$@zhi1$人购买】
		option 102 支持-喀麦隆队【已有$@zhi2$人购买】
		option 103 支持-墨西哥队【已有$@zhi3$人购买】
		option 104 支持-克罗地亚队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 101
	choose 2 301 300 购买【巴西队】需要支付5000银币\n是否确认购买？
endproc

proc 301
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 1
					set_role_var 74 8 @zhi 0
					
					get_global_var 0 16 0 num1
					add num1 1
					set_global_var 0 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【巴西队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 102
	choose 2 302 300 购买【喀麦隆队】需要支付5000银币\n是否确认购买？
endproc

proc 302
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 2
					set_role_var 74 8 @zhi 0
					
					get_global_var 2 16 0 num1
					add num1 1
					set_global_var 2 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【喀麦隆队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 103
	choose 2 303 300 购买【墨西哥队】需要支付5000银币\n是否确认购买？
endproc

proc 303
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 3
					set_role_var 74 8 @zhi 0
					
					get_global_var 4 16 0 num1
					add num1 1
					set_global_var 4 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【墨西哥队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 104
	choose 2 304 300 购买【克罗地亚队】需要支付5000银币\n是否确认购买？
endproc

proc 304
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 4
					set_role_var 74 8 @zhi 0
					
					get_global_var 6 16 0 num1
					add num1 1
					set_global_var 6 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【克罗地亚队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 110
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 8 16 ret zhi1
	get_global_var 10 16 ret zhi2
	get_global_var 12 16 ret zhi3
	get_global_var 14 16 ret zhi4
	call 200
	
		option 111 支持-西班牙队【已有$@zhi1$人购买】
		option 112 支持-智利队【已有$@zhi2$人购买】
		option 113 支持-澳大利亚队【已有$@zhi3$人购买】
		option 114 支持-荷兰队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 111
	choose 2 311 300 购买【西班牙队】需要支付5000银币\n是否确认购买？
endproc

proc 311
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 5
					set_role_var 74 8 @zhi 0
					
					get_global_var 8 16 0 num1
					add num1 1
					set_global_var 8 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【西班牙队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 112
	choose 2 312 300 购买【智利队】需要支付5000银币\n是否确认购买？
endproc

proc 312
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 6
					set_role_var 74 8 @zhi 0
					
					get_global_var 10 16 0 num1
					add num1 1
					set_global_var 10 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【智利队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 113
	choose 2 313 300 购买【澳大利亚队】需要支付5000银币\n是否确认购买？
endproc

proc 313
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 7
					set_role_var 74 8 @zhi 0
					
					get_global_var 12 16 0 num1
					add num1 1
					set_global_var 12 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【澳大利亚队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 114
	choose 2 314 300 购买【荷兰队】需要支付5000银币\n是否确认购买？
endproc

proc 314
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 8
					set_role_var 74 8 @zhi 0
					
					get_global_var 14 16 0 num1
					add num1 1
					set_global_var 14 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【荷兰队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 120
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 16 16 ret zhi1
	get_global_var 18 16 ret zhi2
	get_global_var 20 16 ret zhi3
	get_global_var 22 16 ret zhi4
	call 200
	
		option 121 支持-哥伦比亚队【已有$@zhi1$人购买】
		option 122 支持-科特迪瓦队【已有$@zhi2$人购买】
		option 123 支持-日本队【已有$@zhi3$人购买】
		option 124 支持-希腊队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 121
	choose 2 321 300 购买【哥伦比亚队】需要支付5000银币\n是否确认购买？
endproc

proc 321
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 9
					set_role_var 74 8 @zhi 0
					
					get_global_var 16 16 0 num1
					add num1 1
					set_global_var 16 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【哥伦比亚队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 122
	choose 2 322 300 购买【科特迪瓦队】需要支付5000银币\n是否确认购买？
endproc

proc 322
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 10
					set_role_var 74 8 @zhi 0
					
					get_global_var 18 16 0 num1
					add num1 1
					set_global_var 18 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【科特迪瓦队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 123
	choose 2 323 300 购买【日本队】需要支付5000银币\n是否确认购买？
endproc

proc 323
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 11
					set_role_var 74 8 @zhi 0
					
					get_global_var 20 16 0 num1
					add num1 1
					set_global_var 20 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【日本队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 124
	choose 2 324 300 购买【希腊队】需要支付5000银币\n是否确认购买？
endproc

proc 324
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 12
					set_role_var 74 8 @zhi 0
					
					get_global_var 22 16 0 num1
					add num1 1
					set_global_var 22 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【希腊队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 130
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 24 16 ret zhi1
	get_global_var 26 16 ret zhi2
	get_global_var 28 16 ret zhi3
	get_global_var 30 16 ret zhi4
	call 200
	
		option 131 支持-乌拉圭队【已有$@zhi1$人购买】
		option 132 支持-英格兰队【已有$@zhi2$人购买】
		option 133 支持-哥斯达黎加队【已有$@zhi3$人购买】
		option 134 支持-意大利队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 131
	choose 2 331 300 购买【乌拉圭队】需要支付5000银币\n是否确认购买？
endproc

proc 331
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 13
					set_role_var 74 8 @zhi 0
					
					get_global_var 24 16 0 num1
					add num1 1
					set_global_var 24 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【乌拉圭队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 132
	choose 2 332 300 购买【英格兰队】需要支付5000银币\n是否确认购买？
endproc

proc 332
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 14
					set_role_var 74 8 @zhi 0
					
					get_global_var 26 16 0 num1
					add num1 1
					set_global_var 26 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【英格兰队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 133
	choose 2 333 300 购买【哥斯达黎加队】需要支付5000银币\n是否确认购买？
endproc

proc 333
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 15
					set_role_var 74 8 @zhi 0
					
					get_global_var 28 16 0 num1
					add num1 1
					set_global_var 28 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【哥斯达黎加队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 134
	choose 2 334 300 购买【意大利队】需要支付5000银币\n是否确认购买？
endproc

proc 334
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 16
					set_role_var 74 8 @zhi 0
					
					get_global_var 30 16 0 num1
					add num1 1
					set_global_var 30 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【意大利队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 140
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 32 16 ret zhi1
	get_global_var 34 16 ret zhi2
	get_global_var 36 16 ret zhi3
	get_global_var 38 16 ret zhi4
	call 200
	
		option 141 支持-瑞士队【已有$@zhi1$人购买】
		option 142 支持-厄瓜多尔队【已有$@zhi2$人购买】
		option 143 支持-洪都拉斯队【已有$@zhi3$人购买】
		option 144 支持-法国队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 141
	choose 2 341 300 购买【瑞士队】需要支付5000银币\n是否确认购买？
endproc

proc 341
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 17
					set_role_var 74 8 @zhi 0
					
					get_global_var 32 16 0 num1
					add num1 1
					set_global_var 32 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【瑞士队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 142
	choose 2 342 300 购买【厄瓜多尔队】需要支付5000银币\n是否确认购买？
endproc

proc 342
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 18
					set_role_var 74 8 @zhi 0
					
					get_global_var 34 16 0 num1
					add num1 1
					set_global_var 34 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【厄瓜多尔队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 143
	choose 2 343 300 购买【洪都拉斯队】需要支付5000银币\n是否确认购买？
endproc

proc 343
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 19
					set_role_var 74 8 @zhi 0
					
					get_global_var 36 16 0 num1
					add num1 1
					set_global_var 36 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【洪都拉斯队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 144
	choose 2 344 300 购买【法国队】需要支付5000银币\n是否确认购买？
endproc

proc 344
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 20
					set_role_var 74 8 @zhi 0
					
					get_global_var 38 16 0 num1
					add num1 1
					set_global_var 38 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【法国队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 150
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 40 16 ret zhi1
	get_global_var 42 16 ret zhi2
	get_global_var 44 16 ret zhi3
	get_global_var 46 16 ret zhi4
	call 200
	
		option 151 支持-阿根廷队【已有$@zhi1$人购买】
		option 152 支持-尼日利亚队【已有$@zhi2$人购买】
		option 153 支持-伊朗队【已有$@zhi3$人购买】
		option 154 支持-波黑队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 151
	choose 2 351 300 购买【阿根廷队】需要支付5000银币\n是否确认购买？
endproc

proc 351
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 21
					set_role_var 74 8 @zhi 0
					
					get_global_var 40 16 0 num1
					add num1 1
					set_global_var 40 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【阿根廷队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 152
	choose 2 352 300 购买【尼日利亚队】需要支付5000银币\n是否确认购买？
endproc

proc 352
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 22
					set_role_var 74 8 @zhi 0
					
					get_global_var 42 16 0 num1
					add num1 1
					set_global_var 42 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【尼日利亚队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 153
	choose 2 353 300 购买【伊朗队】需要支付5000银币\n是否确认购买？
endproc

proc 353
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 23
					set_role_var 74 8 @zhi 0
					
					get_global_var 44 16 0 num1
					add num1 1
					set_global_var 44 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【伊朗队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 154
	choose 2 354 300 购买【波黑队】需要支付5000银币\n是否确认购买？
endproc

proc 354
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 24
					set_role_var 74 8 @zhi 0
					
					get_global_var 46 16 0 num1
					add num1 1
					set_global_var 46 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【波黑队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 160
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 48 16 ret zhi1
	get_global_var 50 16 ret zhi2
	get_global_var 52 16 ret zhi3
	get_global_var 54 16 ret zhi4
	call 200
	
		option 161 支持-德国队【已有$@zhi1$人购买】
		option 162 支持-加纳队【已有$@zhi2$人购买】
		option 163 支持-美国队【已有$@zhi3$人购买】
		option 164 支持-葡萄牙队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 161
	choose 2 361 300 购买【德国队】需要支付5000银币\n是否确认购买？
endproc

proc 361
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 25
					set_role_var 74 8 @zhi 0
					
					get_global_var 48 16 0 num1
					add num1 1
					set_global_var 48 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【德国队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 162
	choose 2 362 300 购买【加纳队】需要支付5000银币\n是否确认购买？
endproc

proc 362
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 26
					set_role_var 74 8 @zhi 0
					
					get_global_var 50 16 0 num1
					add num1 1
					set_global_var 50 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【加纳队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 163
	choose 2 363 300 购买【美国队】需要支付5000银币\n是否确认购买？
endproc

proc 363
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 27
					set_role_var 74 8 @zhi 0
					
					get_global_var 52 16 0 num1
					add num1 1
					set_global_var 52 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【美国队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 164
	choose 2 364 300 购买【葡萄牙队】需要支付5000银币\n是否确认购买？
endproc

proc 364
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 28
					set_role_var 74 8 @zhi 0
					
					get_global_var 54 16 0 num1
					add num1 1
					set_global_var 54 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【葡萄牙队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 170
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 56 16 ret zhi1
	get_global_var 58 16 ret zhi2
	get_global_var 60 16 ret zhi3
	get_global_var 62 16 ret zhi4
	call 200
	
		option 171 支持-比利时队【已有$@zhi1$人购买】
		option 172 支持-阿尔及利亚队【已有$@zhi2$人购买】
		option 173 支持-韩国队【已有$@zhi3$人购买】
		option 174 支持-俄罗斯队【已有$@zhi4$人购买】
		option 1 返回
	
	talk
endproc

proc 171
	choose 2 371 300 购买【比利时队】需要支付5000银币\n是否确认购买？
endproc

proc 371
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 29
					set_role_var 74 8 @zhi 0
					
					get_global_var 56 16 0 num1
					add num1 1
					set_global_var 56 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【比利时队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 172
	choose 2 372 300 购买【阿尔及利亚队】需要支付5000银币\n是否确认购买？
endproc

proc 372
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 30
					set_role_var 74 8 @zhi 0
					
					get_global_var 58 16 0 num1
					add num1 1
					set_global_var 58 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【阿尔及利亚队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 173
	choose 2 373 300 购买【韩国队】需要支付5000银币\n是否确认购买？
endproc

proc 373
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 31
					set_role_var 74 8 @zhi 0
					
					get_global_var 60 16 0 num1
					add num1 1
					set_global_var 60 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【韩国队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 174
	choose 2 374 300 购买【俄罗斯队】需要支付5000银币\n是否确认购买？
endproc

proc 374
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 32
					set_role_var 74 8 @zhi 0
					
					get_global_var 62 16 0 num1
					add num1 1
					set_global_var 62 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 恭喜你成功购买【俄罗斯队】，只要该球队晋级即可获得奖励！
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 200
	add_talk 购买说明：
	add_talk #b#b每人#c186506只能购买1次#n，购买需要花费#c005ebe5000银币#n
	add_talk #b#b购买截止日期到#cF80B0B6月12日24:00#n
	add_talk #b#b
	add_talk 晋级奖励：
	add_talk #b#b比赛共分为#c005ebe五个阶段#n，每阶段每队一万金券
	add_talk #b#b只要购买的球队晋级，即可获得丰厚的金券
	add_talk #b#b晋级球队奖励的金券，将平均分给该队买家
	add_talk #b#b购买晋级球队的玩家，至少可获得#cff4e00一百金券#n
	strvar zhi
	GET_ROLE_VAR 74 8 0 zhi
	if zhi = 1
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【巴西队】#n
	endif
	if zhi = 2
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【喀麦隆队】#n
	endif
	if zhi = 3
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【墨西哥队】#n
	endif
	if zhi = 4
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【克罗地亚队】#n
	endif
	if zhi = 5
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【西班牙队】#n
	endif
	if zhi = 6
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【智利队】#n
	endif
	if zhi = 7
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【澳大利亚队】#n
	endif
	if zhi = 8
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【荷兰队】#n
	endif
	if zhi = 9
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【哥伦比亚队】#n
	endif
	if zhi = 10
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【科特迪瓦队】#n
	endif
	if zhi = 11
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【日本队】#n
	endif
	if zhi = 12
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【希腊队】#n
	endif
	if zhi = 13
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【乌拉圭队】#n
	endif
	if zhi = 14
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【英格兰队】#n
	endif
	if zhi = 15
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【哥斯达黎加队】#n
	endif
	if zhi = 16
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【意大利队】#n
	endif
	if zhi = 17
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【瑞士队】#n
	endif
	if zhi = 18
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【厄瓜多尔队】#n
	endif
	if zhi = 19
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【洪都拉斯队】#n
	endif
	if zhi = 20
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【法国队】#n
	endif
	if zhi = 21
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【阿根廷队】#n
	endif
	if zhi = 22
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【尼日利亚队】#n
	endif
	if zhi = 23
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【伊朗队】#n
	endif
	if zhi = 24
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【波黑队】#n
	endif
	if zhi = 25
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【德国队】#n
	endif
	if zhi = 26
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【加纳队】#n
	endif
	if zhi = 27
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【美国队】#n
	endif
	if zhi = 28
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【葡萄牙队】#n
	endif
	if zhi = 29
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【比利时队】#n
	endif
	if zhi = 30
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【阿尔及利亚队】#n
	endif
	if zhi = 31
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【韩国队】#n
	endif
	if zhi = 32
		add_talk #b#b
		add_talk #c005ebe★. 你已经购买了【俄罗斯队】#n
	endif
endproc

proc 201
	ADD_SYS_MSG_START
	ADD_SYS_MSG 购买截止日期已过，这次世界杯无法再购买
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc

proc 202
	ADD_SYS_MSG_START
	ADD_SYS_MSG 你已经购买了支持的球队，这次世界杯无法再购买
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc

proc 203
	ADD_SYS_MSG_START
	ADD_SYS_MSG 身上携带的银币不足，暂时无法购买
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
