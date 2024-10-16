proc 1
	strvar num
	GET_RONGLU_TASK_NUM 10 140 num
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我有一个聚宝盆，秋天的时候把它种在地里，冬天就收获了好多好多紫色装备。
	; add_talk #b#b我有一个聚宝盆，秋天的时候把它种在地里，冬天就收获了好多好多稀有宝石。
	; add_talk #b#b我有一个聚宝盆，秋天的时候把它种在地里，冬天就收获了好多好多珍贵材料。
	add_talk #b#b哦，不对，是我有一个#cFF0080魔法熔炉#n……

	if 10 = 0 
	;	add_talk #b#b#c186506宝鉴-熔炉秘宝任务每天可完成6次
	;	add_talk #b#b#c186506拥有翡翠徽章特权的角色可额外完成3次
	;	add_talk #b#b
		if num <= 6
			add_talk #b#b#c186506<当前已完成$@num$/6次任务>
			add_talk #b#b<翡翠徽章额外完成0/3次任务>
			
		else
		
			add_talk #b#b#c186506<当前已完成6/6次任务>
			sub num 6
			add_talk #b#b<翡翠徽章额外完成$@num$/3次任务>
		endif
		
		add_talk #b#b<建议组队进行任务>
		
	endif

		;;;;;;;;;;;;
		task_add_option	3250 1 103
		task_add_option	3251 1 104
		task_add_option	3252 1 105
		task_add_option	3253 1 106
		task_add_option	3254 1 107
		
		
		option 150 #重铸熔炉品质
		
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		; strvar num1 num2 num3 num4 num5 
		; GET_ITEM_NUM_INPAK 5052316 1 num1
		; GET_ITEM_NUM_INPAK 5052317 1 num2
		; GET_ITEM_NUM_INPAK 5052318 1 num3
		; GET_ITEM_NUM_INPAK 5052319 1 num4
		; GET_ITEM_NUM_INPAK 5052328 1 num5	
		
		; if 20 = 2
			; option 250 #换取金熔炉（拥有凭证$@mum1$/10）
		; elseif 21 = 2
			; option 250 #换取金熔炉（拥有凭证$@mum2$/10）
		
		; elseif 22 = 2
			; option 250 #换取金熔炉（拥有凭证$@mum3$/10）
		
		; elseif 23 = 2
			; option 250 #换取金熔炉（拥有凭证$@mum4$/10）
		
		; elseif 24 = 2
			; option 250 #换取金熔炉（拥有凭证$@mum5$/10）
		; else
			option 250 #兑换金熔炉
	;	endif
		
		
		option 260 #兑换金熔炉凭证
		
		if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2 or 24 = 2
			option 200 时空传送
		endif
	talk
	
	
endproc 



;;;;;;;;;;;;;;;;;
PROC 103
	task_do_option 3250 1
ENDPROC
PROC 104
	task_do_option 3251 1
ENDPROC
PROC 105
	task_do_option 3252 1
ENDPROC
PROC 106
	task_do_option 3253 1
ENDPROC
PROC 107
	task_do_option 3254 1
ENDPROC

proc 250
	dialog 1
	add_talk $_npc$:
	add_talk #b#b朋友，寻宝的路是很漫长的，看得出来你很努力。其实我这有几个金熔炉，但数量不多，如果你有一些“金熔炉凭证”我倒是能借给你用一用……
	add_talk #b#b
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		
		strvar num1 num2 num3 num4 num5 a
		GET_ITEM_NUM_INPAK 5052316 1 num1
		GET_ITEM_NUM_INPAK 5052317 1 num2
		GET_ITEM_NUM_INPAK 5052318 1 num3
		GET_ITEM_NUM_INPAK 5052319 1 num4
		GET_ITEM_NUM_INPAK 5052328 1 num5	
	if 20 = 2
		add_talk #b#b当前你接取的任务为：#c186506 40级宝鉴-熔炉秘宝任务#n
		add_talk #b#b换取40~49级宝鉴任务-金熔炉需要#c186506 10 #n个【40级金熔炉凭证】
		add_talk #b#b
		add_talk #b#b背包中有#c186506 $@num1$ #n个【40级金熔炉凭证】
		var a @num1
		dev @a 10
		add_talk #b#b当前可以换取#c186506 $@a$ #n个金熔炉
	endif
	if 21 = 2
		add_talk #b#b当前你接取的任务为：#c186506 50级宝鉴-熔炉秘宝任务#n
		add_talk #b#b换取50~59级宝鉴任务-金熔炉需要#c186506 20 #n个【50级金熔炉凭证】
		add_talk #b#b
		add_talk #b#b背包中有#c186506 $@num2$ #n个【50级金熔炉凭证】
		var a @num2
		dev @a 20
		add_talk #b#b当前可以换取#c186506 $@a$ #n个金熔炉
		
	endif
	if 22 = 2
		add_talk #b#b当前你接取的任务为：#c186506 60级宝鉴-熔炉秘宝任务#n
		add_talk #b#b换取60~69级宝鉴任务-金熔炉需要#c186506 30 #n个【60级金熔炉凭证】
		add_talk #b#b
		add_talk #b#b背包中有#c186506 $@num3$ #n个【60级金熔炉凭证】
		var a @num3
		dev @a 30
		add_talk #b#b当前可以换取#c186506 $@a$ #n个金熔炉
	endif
	if 23 = 2
		add_talk #b#b当前你接取的任务为：#c186506 70级宝鉴-熔炉秘宝任务#n
		add_talk #b#b换取70~79级宝鉴任务-金熔炉需要#c186506 40 #n个【70级金熔炉凭证】
		add_talk #b#b
		add_talk #b#b背包中有#c186506 $@num4$ #n个【70级金熔炉凭证】
		var a @num4
		dev @a 40
		add_talk #b#b当前可以换取#c186506 $@a$ #n个金熔炉
	endif
	if 24 = 2
		add_talk #b#b当前你接取的任务为：#c186506 80级宝鉴-熔炉秘宝任务#n
		add_talk #b#b换取80级以上宝鉴任务-金熔炉需要#c186506 50 #n个【80级金熔炉凭证】
		add_talk #b#b
		add_talk #b#b背包中有#c186506 $@num5$ #n个【80级金熔炉凭证】
		var a @num5
		dev @a 50
		add_talk #b#b当前可以换取#c186506 $@a$ #n个金熔炉
	endif
		
		option 255 #确定换取金熔炉
		option 1 返回
	talk
endproc
proc 255

		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		if 20 = 1 or 21 = 1 or 22 = 1 or 23 = 1 or 24 = 1
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你已经完成了宝鉴-熔炉秘宝任务，无法换取金熔炉
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
			return
		endif
		strvar num1 num2 num3 num4 num5
		GET_ITEM_NUM_INPAK 5052316 1 num1
		GET_ITEM_NUM_INPAK 5052317 1 num2
		GET_ITEM_NUM_INPAK 5052318 1 num3
		GET_ITEM_NUM_INPAK 5052319 1 num4
		GET_ITEM_NUM_INPAK 5052328 1 num5	
		if 20 = 2 

			; GET_PLAYER_BASE_ATT 1 1 30 1 
			; if 30 >= 40 and 30 < 50
			
			if num1 >= 10
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052316 10 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 21 = 2
			if num2 >= 20
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052317 20 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 22 = 2
			;if 30 >= 60 and 30 < 70
			if num3 >= 30
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052318 30 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 23 = 2
			if num4 >= 40
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052319 40 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		
		elseif 24 = 2
			if num5 >= 50
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052328 50 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你还没有接取宝鉴-熔炉秘宝任务
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
endproc
proc 256
	ADD_SYS_MSG_START
	ADD_SYS_MSG 与任务等级匹配的金熔炉凭证不够，无法兑换金熔炉
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc 
proc 257
	ADD_SYS_MSG_START
	ADD_SYS_MSG 背包内没有可以换取的熔炉
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc 


proc 260
	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b
	add_talk #cFF0080兑换规则：#n
	add_talk #b#b
	add_talk #c186506 5个#n40级金熔炉凭证#cFF0080兑换#n#c186506 1个#n 50级金熔炉凭证
	add_talk #c186506 5个#n50级金熔炉凭证#cFF0080兑换#n#c186506 1个#n 60级金熔炉凭证
	add_talk #c186506 5个#n60级金熔炉凭证#cFF0080兑换#n#c186506 1个#n 70级金熔炉凭证
	add_talk #c186506 5个#n70级金熔炉凭证#cFF0080兑换#n#c186506 1个#n 80级金熔炉凭证
;	add_talk #c186506 5个#n50级金熔炉凭证#cFF0080兑换#n#c186506 1个#n 60级金熔炉凭证
	
	
	
		option 261 兑换50级金熔炉凭证
		option 262 兑换60级金熔炉凭证
		option 263 兑换70级金熔炉凭证
		option 264 兑换80级金熔炉凭证
		option 1 返回
		
	talk
endproc

proc 261
	strvar num1 num2 num3 num4 num5
	GET_ITEM_NUM_INPAK 5052316 1 num1
	if num1 >= 5
		
		DEC_ITEM_NUM_INPAK 5052316 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052317 1 1 0
		
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换所需要的40级金熔炉凭证不足5张，兑换失败
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
proc 262

	GET_ITEM_NUM_INPAK 5052317 1 num2
	if num2 >= 5
	
		DEC_ITEM_NUM_INPAK 5052317 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052318 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换所需要的50级金熔炉凭证不足5张，兑换失败
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	
	endif

endproc
proc 263
	GET_ITEM_NUM_INPAK 5052318 1 num3
	if num3 >= 5
	
		DEC_ITEM_NUM_INPAK 5052318 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052319 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换所需要的60级金熔炉凭证不足5张，兑换失败
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 
proc 264

	GET_ITEM_NUM_INPAK 5052319 1 num4
	if num4 >= 5
	
		DEC_ITEM_NUM_INPAK 5052319 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052328 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 兑换所需要的70级金熔炉凭证不足5张，兑换失败
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	
	endif
endproc


proc 200
	dialog 1
	add_talk $_npc$:
	
	add_talk #b#b我可以把你送到任务地图，不过我可要收取一点手续费哦！
	add_talk #b#b不过还有一种不用花钱的方法，可以从自己公会城市的公会枢纽消耗贡献度传送到任务地图
;	add_talk 传送花费（绑银不足自动消耗银币）：
;	add_talk #b#b#c186506落日荒野（250绑银）
;	add_talk #b#b越雾雨林（350绑银）
;	add_talk #b#b荫川林地（500绑银）
;	add_talk #b#b风回裂谷（800绑银）
;	add_talk #b#b鸣沙陷地（810绑银）#n
	add_talk #b#b
	add_talk #c0181f9你当前的任务地点为：
	task_stat 3250 1 20
	if 20 = 2
		add_talk #b#b落日荒野，需要花费#c186506 250 #n#c0181f9绑银
		option 300 #我要去杀邪骸党哨兵	
		option 301 #我要去杀矿坑感染者
	endif
	task_stat 3251 1 30
	if 30 = 2
		add_talk #b#b越雾雨林，需要花费#c186506 350 #n#c0181f9绑银
		option 310 #我要去杀玛吉战士
		option 311 #我要去杀丛林猎豹
	endif
	task_stat 3252 1 40
	if 40 = 2
		add_talk #b#b荫川林地，需要花费#c186506 500 #n#c0181f9绑银
		option 320 #我要去杀卡鲁亚魔像
		option 321 #我要去杀铁翼士兵
	endif
	task_stat 3253 1 50
	if 50 = 2
		add_talk #b#b风回裂谷，需要花费#c186506 800 #n#c0181f9绑银
		option 330 #我要去杀噬灵怪
		option 331 #我要去杀覆霜破坏者
	endif
	task_stat 3254 1 60
	if 60 = 2
		add_talk #b#b迈阿荒原，需要花费#c186506 3000 #n#c0181f9绑银
		option 340 #我要去杀被感染的村民
		option 341 #我要去杀被感染的矿工
	endif
	
		option 1 返回
		
	talk
endproc 

proc 300
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 210 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 250
		choose 1 210 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 210
	endif
endproc 
proc 301
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 220 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 250
		choose 1 220 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 310
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 210 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 350
		choose 1 210 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 210
	endif
endproc 
proc 311
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 220 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 350
		choose 1 220 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 320
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 500 and @ret >= 500
		choose 1 210 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 500
		choose 1 210 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 210
	endif
endproc 
proc 321
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 500 and @ret >= 500
		choose 1 220 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 500
		choose 1 220 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 330
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 800 and @ret >= 800
		choose 1 210 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 800
		choose 1 210 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 210
	endif
endproc 
proc 331
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 800 and @ret >= 800
		choose 1 220 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 800
		choose 1 220 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 340
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 3000 and @ret >= 3000
		choose 1 210 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 3000
		choose 1 210 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 210
	endif
endproc 
proc 341
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 3000 and @ret >= 3000
		choose 1 220 221 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 3000
		choose 1 220 221 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;


proc 221
endproc
proc 210
	strvar Moy
	task_stat 3250 1 20
	if 20 = 2
		DEC_MONEY_TEST 0 250 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 250 21 10
			if 10 = 0
				TRANSMIT 2400 6100 15600 1
			endif
		endif
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		DEC_MONEY_TEST 0 350 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 350 21 10
			if 10 = 0
				TRANSMIT 2500 13200 28200 1 
			endif
		endif
	endif
	task_stat 3252 1 40
	if 40 = 2
		DEC_MONEY_TEST 0 500 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 500 21 10
			if 10 = 0
				TRANSMIT 2104 18700 14400 1 
			endif
		endif
	endif
	task_stat 3253 1 50
	if 50 = 2
		DEC_MONEY_TEST 0 800 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 800 21 10
			if 10 = 0
				TRANSMIT 2201 9700 9000 1 
			endif
		endif
	endif
	task_stat 3254 1 60
	if 60 = 2
		DEC_MONEY_TEST 0 3000 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 3000 21 10
			if 10 = 0
				TRANSMIT 3402 15900 21300 1
			endif
		endif
	endif
endproc  


proc 220
	strvar Moy
	task_stat 3250 1 20
	if 20 = 2
		DEC_MONEY_TEST 0 250 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 250 21 10
			if 10 = 0
				TRANSMIT 2400 16900 16600 1
			endif
		endif
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		DEC_MONEY_TEST 0 350 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 350 21 10
			if 10 = 0
				TRANSMIT 2500 28100 30000 1 
			endif
		endif
	endif
	task_stat 3252 1 40
	if 40 = 2
		DEC_MONEY_TEST 0 500 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 500 21 10
			if 10 = 0
				TRANSMIT 2104 23600 11200 1 
			endif
		endif
	endif
	task_stat 3253 1 50
	if 50 = 2
		DEC_MONEY_TEST 0 800 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 800 21 10
			if 10 = 0
				TRANSMIT 2201 23100 33100 1 
			endif
		endif
	endif
	task_stat 3254 1 60
	if 60 = 2
		DEC_MONEY_TEST 0 3000 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的银币不足，无法传送！
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 3000 21 10
			if 10 = 0
				TRANSMIT 3402 28000 6800 1
			endif
		endif
	endif
endproc  


proc 2
	get_trig_task 1 2
	if 2 = 3250 
		call 1
	endif
	if 2 = 3251
		call 1
	endif
	if 2 = 3252
		call 1
	endif
	if 2 = 3253
		call 1
	endif
	if 2 = 3254
		call 1
	endif
	
endproc

proc 150
	open_ui 75
	
endproc 








