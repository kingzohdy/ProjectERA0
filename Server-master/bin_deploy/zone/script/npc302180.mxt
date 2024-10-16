;布鲁鲁

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你要【#c186506万圣节糖果#n】吗？
	add_talk #b#b如果你想要，你可以跟我说。
	add_talk #b#b你不跟我说，我怎么知道你想要呢。你跟我说，我一定给你。
	add_talk #b#b其实你要不要糖果了？
	
		OPTION 200 #我要糖果！

	talk
	call 300
endproc

proc 200
	DIALOG 1
	STRVAR cs
	
	add_talk $_npc$:
	add_talk #b#b1.每间隔30分钟可免费领取1次。
	add_talk #b#b2.如果你能给点金币，那~~糖果请随便拿。
	add_talk #b#b
	
	GET_ROLE_VAR 58 8 0 cs
	if cs = 1
		add_talk #b#b支付#cFF4e002金币#n可再领取1颗糖果。
	elseif cs = 2
		add_talk #b#b支付#cFF4e005金币#n可再领取1颗糖果。
	elseif cs = 3
		add_talk #b#b支付#cFF4e0010金币#n可再领取1颗糖果。
	elseif cs = 4
		add_talk #b#b支付#cFF4e0020金币#n可再领取1颗糖果。
	elseif cs = 5
		add_talk #b#b支付#cFF4e0050金币#n可再领取1颗糖果。
	elseif cs = 6
		add_talk #b#b支付#cFF4e00100金币#n可再领取1颗糖果。
	elseif cs > 6
		add_talk #b#b支付#cFF4e00200金币#n可再领取1颗糖果。
	else
	endif
	
	STRVAR sec js sj
	
	GET_TIME_SEC sec
	GET_ROLE_VAR 59 32 0 js
	
	sub sec @js
	var sj 1800
	sub sj @sec
	; dev sj 60
	if sj > 0 and sj <= 1800
		add_talk #b#b距离下次免费领取还需要#c186506$@sj$#n秒
	endif
	
	
		OPTION 201 #马上给我来个糖果
		OPTION 1 返回

	talk
	call 300
endproc

proc 201
	STRVAR cs
	
	GET_ROLE_VAR 58 8 0 cs ;获取领取次数
	if cs < 1
		call 202
	elseif cs = 1
		choose 1 202 203 免费领取冷却时间未到，是否花费2金币兑换?
	elseif cs = 2
		choose 1 202 203 免费领取冷却时间未到，是否花费5金币兑换?
	elseif cs = 3
		choose 1 202 203 免费领取冷却时间未到，是否花费10金币兑换?
	elseif cs = 4
		choose 1 202 203 免费领取冷却时间未到，是否花费20金币兑换?
	elseif cs = 5
		choose 1 202 203 免费领取冷却时间未到，是否花费50金币兑换?
	elseif cs = 6
		choose 1 202 203 免费领取冷却时间未到，是否花费100金币兑换?
	elseif cs > 6
		choose 1 202 203 免费领取冷却时间未到，是否花费200金币兑换?
	else
	endif
	
	call 300
endproc

PROC 202

	STRVAR ret ret1 ret2 kg cs js jb
	
	GET_PACK_SPACE 0 kg ;获取背包剩余格数
	
	if kg < 1
		add_sys_msg_start
		add_sys_msg 空出1个背包格子再来找我哦
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	CALL 300
	
	GET_ROLE_VAR 58 8 0 cs ;获取领取次数	
	if cs < 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
			if ret2 = 0
				GET_TIME_SEC js ;获取当前时间
					
				; 记录当前时间
				SET_ROLE_VAR 59 32 @js 0
					
				; 记录领取次数
				add cs 1
				SET_ROLE_VAR 58 8 @cs 0
				CALL 200
			endif
	elseif cs = 1
		var jb 2
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0 
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 2
		var jb 5
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 3
		var jb 10
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 4
		var jb 20
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 5
		var jb 50
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 6
		var jb 100
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; 记录领取次数
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs > 6
		var jb 200
		DEC_MONEY_TEST 0 @jb 60 ret ;检测携带金币数量
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;扣除相应金币数量
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
				CALL 200
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足，每30分钟只能领取1次糖果
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
	endif
ENDPROC

proc 203

endproc

proc 300
	STRVAR sec cs js
	
	GET_TIME_SEC sec
	GET_ROLE_VAR 58 8 0 cs
	GET_ROLE_VAR 59 32 0 js
	
	sub sec @js
	if sec > 1800
		var cs 0
		SET_ROLE_VAR 58 8 @cs 0
		var js 0
		SET_ROLE_VAR 59 32 @js 0
	endif
endproc

