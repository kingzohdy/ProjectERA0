;游乐场
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临急你所急航空，我公司禀承“客户至上”的承诺，通过提供“可靠、准点、方便、快捷”的优质服务，致力满足并超越客户的期望，更曾一度获得民航总局颁发的飞行安全最高奖。
;	add_talk #b#b如今，为答谢广大客户的支持与厚爱，我公司特举办飞行优惠活动，凡在时间城内搭乘我公司的航班，无论距离远近，只需要90银币！
;	add_talk #b#b只需90银币！您就可以缩短大量的路程！您没有看错！不是9千！不是9百！只要90银币！
	
	option 105 时间城内航线
	option 104 #世界航线

	talk
	
endproc
proc 105
	dialog 1
	add_talk $_npc$:
	add_talk #b#b如今，为答谢广大客户的支持与厚爱，我公司特举办飞行优惠活动，凡搭乘我公司的时间城内航班，无论远近，马上就能到达！

	OPTION 50 #去工匠之家
	OPTION 60 #去第九街区
	OPTION 70 #去自由港
	OPTION 80 #去英雄广场

	talk
endproc
proc 104
	MAP_TRANS 1
endproc
proc 50
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 101 99 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 90
		choose 1 101 99 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 101
	endif
endproc 
proc 99
	return
endproc

proc 101
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 您背包中的银币不足，无法乘坐。
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 106 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc
proc 60
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 102 99 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 90
		choose 1 102 99 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 102
	endif
endproc 
proc 102
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 您背包中的银币不足，无法乘坐。
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 108 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc
proc 70
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 103 99 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 90
		choose 1 103 99 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 103
	endif
endproc 
proc 103
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 您背包中的银币不足，无法乘坐。
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 201 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc

proc 80
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 50 and @ret >= 50
		choose 1 106 99 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 50
		choose 1 106 99 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 106
	endif
endproc 
proc 106
	
	RIDE_OFF 1
	strvar RetB Moy
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1
		strvar mid
		DEC_MONEY_TEST 0 50 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 您背包中的银币不足，无法乘坐。
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			GET_PLAYER_ID 1 mid
			TRANSMIT_CAMERA @mid 90 1000 23500 33500 0 0
			DEC_MONEY 0 50 21 10
		endif
	endif
endproc