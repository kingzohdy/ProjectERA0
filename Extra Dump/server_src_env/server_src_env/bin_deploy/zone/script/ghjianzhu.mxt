;公会枢纽


proc 1 

	strvar Ret
	is_in_my_clan_city  Ret
	if Ret = 0 
		return
	endif
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b这是公会的象征，如果保护不了枢纽，那么公会也得不到安定……
	add_talk #b#b不过枢纽也可以提供一些小功能给会员使用
	add_talk #b#b
	add_talk #b#b如果你用城市传送卷回来的，这里也会把你送回到之前的地方…
	add_talk #b#b如果接了魔法熔炉任务，也可以从我这里传送过去
		
		option 150 送我回去吧
		
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2 or 24 = 2
			option 160 #熔炉秘宝传送
		endif
		strvar map
		HAS_STRONG_GLORY 30 map
		strvar mapid
		has_strong_glory 1 mapid
		
		if 30 = 0 and map = 2202 and mapid = 2202
			option 300 传送到冰岛【公会旗帜】（支付130绑银）
		endif
		if 30 = 0 and map = 2200 and mapid = 2200
			option 301 传送到银风雪原【公会旗帜】（支付220绑银）
		endif
		if 30 = 0 and map = 2300 and mapid = 2300
			option 302 传送到流金沙漠【公会旗帜】（支付100绑银）
		endif
		if 30 = 0 and map = 2400 and mapid = 2400
			option 303 传送到落日荒野【公会旗帜】（支付250绑银）
		endif
		if 30 = 0 and map = 2500 and mapid = 2500
			option 304 传送到越雾雨林【公会旗帜】（支付350绑银）
		endif
		if 30 = 0 and map = 3404 and mapid = 3404
			option 305 传送到鸣沙陷地【公会旗帜】（支付810绑银）
		endif
		
	talk
 endproc 
 proc 150
 	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b这里会把你送到进入公会城市之前的地方，确定要回去吗？
		option 151 #我要回去
	talk
endproc
proc 151
	OUT_PWORLD_BY_SINGLE
endproc
 
proc 160
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b如果你接取了熔炉任务，那么你可以在这里传送出去，不过要消耗一点贡献度哦……
	add_talk #b#b
	add_talk #c0181f9你当前的任务地点为：
	task_stat 3250 1 20
	if 20 = 2
		add_talk #b#b落日荒野，需要花费#c186506 3 #n#c0181f9点贡献度
		option 170 #我要去杀邪骸党哨兵	
		option 180 #我要去杀矿坑感染者
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		add_talk #b#b越雾雨林，需要花费#c186506 4 #n#c0181f9点贡献度
		option 170 #我要去杀玛吉战士
		option 180 #我要去杀丛林猎豹
	endif
	task_stat 3252 1 40
	if 40 = 2
		add_talk #b#b荫川林地，需要花费#c186506 9 #n#c0181f9点贡献度
		option 170 #我要去杀卡鲁亚魔像
		option 180 #我要去杀铁翼士兵
	endif
	task_stat 3253 1 50
	if 50 = 2
		add_talk #b#b风回裂谷，需要花费#c186506 15 #n#c0181f9点贡献度
		option 170 #我要去杀噬灵怪
		option 180 #我要去杀覆霜破坏者
	endif
	task_stat 3254 1 60
	if 60 = 2
		add_talk #b#b迈阿荒原，需要花费#c186506 20 #n点贡献度
		option 170 #我要去杀被感染的村民
		option 180 #我要去杀被感染的矿工
	endif
	
		option 1 返回
	talk
endproc 
proc 170
	task_stat 3250 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 3
			ADD_ID_PLAYER_CONTRI 11 0 -3 0 0
			if 11 = 0
				TRANSMIT 2400 6100 15600 1
			endif
		else
			call 200
		endif
	endif
	
	task_stat 3251 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 4
			ADD_ID_PLAYER_CONTRI 11 0 -4 0 0
			if 11 = 0
				TRANSMIT 2500 13200 28200 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3252 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 9
			ADD_ID_PLAYER_CONTRI 11 0 -9 0 0
			if 11 = 0
				TRANSMIT 2104 18700 14400 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3253 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 15
			ADD_ID_PLAYER_CONTRI 11 0 -15 0 0
			if 11 = 0
				TRANSMIT 2201 9700 9000 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3254 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 20
			ADD_ID_PLAYER_CONTRI 11 0 -20 0 0
			if 11 = 0
				TRANSMIT 3402 15900 21300 1
			endif
		else
			call 200
		endif
	endif
endproc

proc 180
	task_stat 3250 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 3
			ADD_ID_PLAYER_CONTRI 11 0 -3 0 0
			if 11 = 0
				TRANSMIT 2400 16900 16600 1
			endif
		else
			call 200
		endif
	endif
	
	task_stat 3251 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 4
			ADD_ID_PLAYER_CONTRI 11 0 -4 0 0
			if 11 = 0
				TRANSMIT 2500 28100 30000 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3252 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 9
			ADD_ID_PLAYER_CONTRI 11 0 -9 0 0
			if 11 = 0
				TRANSMIT 2104 23600 11200 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3253 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 15
			ADD_ID_PLAYER_CONTRI 11 0 -15 0 0
			if 11 = 0
				TRANSMIT 2201 23100 33100 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3254 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 20
			ADD_ID_PLAYER_CONTRI 11 0 -20 0 0
			if 11 = 0
				TRANSMIT 3402 28000 6800 1
			endif
		else
			call 200
		endif
	endif
endproc

 proc 200
	ADD_SYS_MSG_START
	ADD_SYS_MSG 你的贡献度不足，无法传送！
	SET_SYS_MSG_MODE 4 0 0
	MSG_INFO
 endproc 
 
proc 299

endproc 
proc 300
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 130 and @ret >= 130
		choose 1 310 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 130
		choose 1 310 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 310
	endif
endproc
proc 301
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 220 and @ret >= 220
		choose 1 311 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 220
		choose 1 311 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 311
	endif
endproc
proc 302
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 100 and @ret >= 100
		choose 1 312 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 100
		choose 1 312 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 312
	endif
endproc
proc 303
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 313 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 250
		choose 1 313 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 313
	endif
endproc
proc 304
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 314 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 350
		choose 1 314 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 314
	endif
endproc
proc 305
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于10级，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	if 30 < 70
		
		ADD_SYS_MSG_START
		ADD_SYS_MSG 人物等级小于地图等级限制，无法传送
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 810 and @ret >= 810
		choose 1 315 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 810
		choose 1 315 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 315
	endif
endproc



proc 310
	strvar Moy
	DEC_MONEY_TEST 0 130 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 130 21 10
		if 10 = 0
			TRANSMIT 2202 23000 8700 1
		endif
	endif
endproc 
proc 311
	DEC_MONEY_TEST 0 220 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 220 21 10
		if 10 = 0
			TRANSMIT 2200 6500 18000 1
		endif
	endif
endproc 
proc 312
	DEC_MONEY_TEST 0 100 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 100 21 10
		if 10 = 0
			TRANSMIT 2300 8700 12400 1
		endif
	endif
endproc 
proc 313
	DEC_MONEY_TEST 0 250 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 250 21 10
		if 10 = 0
			TRANSMIT 2400 14000 15400 1
		endif
	endif
endproc 
proc 314
	DEC_MONEY_TEST 0 350 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 350 21 10
		if 10 = 0
			TRANSMIT 2500 25700 25700 1
		endif
	endif
endproc 
proc 315
	DEC_MONEY_TEST 0 810 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的银币不足，无法传送！
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 810 21 10
		if 10 = 0
			TRANSMIT 3404 27400 13200 1
		endif
	endif
endproc 








 
 
 
 