

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	add_talk $_npc$:
	if ClanRet = 0
		add_talk #b#b外来人，你别想在我嘴中得到什么宝藏的信息……
	else
		add_talk #b#b我知道公会宝藏的秘密，不过你们想要找到的话，还得下点功夫的！
		
		strvar pos ret ret2 Ret3
		player_clan_pos_info ret 0 0 0 pos
		GET_EVENT_STAT ret2 66
		DYN_ACTIVE_STATUS_GET 1 19 ret3
		
		if ret = 0 and pos != 0 and ret2 = 1 and ret3 = 0
			option 150 #【公会宝藏】活动（未开启）
		elseif  ret = 0 and pos != 0 and ret3 = 1
			option 150 #【公会宝藏】活动（已开启）
		elseif  ret = 0 and pos != 0 and ret2 = 1 and ret3 = 2
			option 150 #【公会宝藏】活动（已结束）
		else
		endif	
	endif
	talk
endproc 

proc 165
	
	ADD_EXP 0 100 0
endproc 

proc 150
	strvar time pos ret ret2 ret3
	get_now_time 1 1 1 1 time 1 1
	GET_EVENT_STAT ret 66
	DYN_ACTIVE_STATUS_GET 1 19 ret2
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b公会研发的宝藏探测器，可以探测出埋藏在公会城市地底的宝藏。
	add_talk #c005ebe【观察者】：普通的宝藏探测器#n
	add_talk #c005ebe【土壤号】：需要公会研发“高能探测技术”#n
;	add_talk #c005ebe【探路者】：需要公会研发“超能探测技术”#n
	if ret = 1 and ret2 = 0
		add_talk #b#b
		add_talk #c186506公会要求：2级公会
		add_talk 活动时间：周一、周三、周五各1次(06:00-24:00)
		add_talk 会长开启：06:00-22:00
		add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
		add_talk 活动进展：未开启
		if time >= 6 and time < 24
			option 157 #召唤宝藏探测器-【观察者】
			option 158 #召唤宝藏探测器-【土壤号】
			; option 159 #召唤宝藏探测器-【探路者】
		endif
	elseif ret2 = 1
			add_talk #b#b
			add_talk #c186506活动时间：周一、周三、周五各1次(06:00-24:00)
			add_talk 活动进展：进行中
			
			option 151 #领取【机械挖掘器】
	elseif ret = 1 and ret2 = 2
			add_talk #c186506活动时间：周一、周三、周五各1次(06:00-24:00)
			add_talk 会长开启：06:00-22:00
			add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
			add_talk 活动进展：已结束#n
	else
	endif

		option 1 返回
	talk
endproc 
proc 157
	strvar time ret ret2 time2 ret3 pos ret4
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 160 153 是否确认开启【公会宝藏】活动并召唤【观察者】？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 160 153 是否确认开启【公会宝藏】活动并召唤【观察者】？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 
proc 158
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 161 153 是否确认开启【公会宝藏】活动并召唤【土壤号】？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 161 153 是否确认开启【公会宝藏】活动并召唤【土壤号】？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 
proc 159
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 162 153 是否确认开启【公会宝藏】活动并召唤【探路者】？
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 162 153 是否确认开启【公会宝藏】活动并召唤【探路者】？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 

proc 160
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad 
			rand rad 3
			switch rad
				case 0
					NEW_CLAN_MON 307012 1 12800 29500 0 500 0 0 1200 rets 1
				endcase
				case 1
					NEW_CLAN_MON 307012 1 12800 29500 0 501 0 0 1200 rets 1
				endcase
				case 2
					NEW_CLAN_MON 307012 1 12800 29500 0 502 0 0 1200 rets 1
				endcase
			endswitch
			if rets = 0
				call 200
				;记录召唤的探测器是哪个
				strvar Locg
				get_pworld_var 64 8 1 Locg
				var Locg 10
				set_pworld_var 64 8 @Locg 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			rand rad 3
			switch rad
				case 0
					NEW_CLAN_MON 307012 1 12800 29500 0 500 0 0 1200 rets 1
				endcase
				case 1
					NEW_CLAN_MON 307012 1 12800 29500 0 501 0 0 1200 rets 1
				endcase
				case 2
					NEW_CLAN_MON 307012 1 12800 29500 0 502 0 0 1200 rets 1
				endcase
			endswitch
			if rets = 0
				call 200
				;记录召唤的探测器是哪个
				strvar Locg
				get_pworld_var 64 8 1 Locg
				var Locg 10
				set_pworld_var 64 8 @Locg 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 161
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad TechLv
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv >= 1
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307013 1 12800 29500 0 500 0 0 1500 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307013 1 12800 29500 0 501 0 0 1500 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307013 1 12800 29500 0 502 0 0 1500 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 20
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg 公会未研发“高能探测技术”科技，无法召唤【土壤号】探测器
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv >= 1
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307013 1 12800 29500 0 500 0 0 1500 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307013 1 12800 29500 0 501 0 0 1500 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307013 1 12800 29500 0 502 0 0 1500 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 20
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg 公会未研发“高能探测技术”科技，无法召唤【土壤号】探测器
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 162
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已开启，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg 【公会宝藏】已结束，今天无法再召唤探测器
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg 公会等级不足，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg 建立公会当天，无法开启【公会宝藏】活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg 不在活动开启时间内，无法开启【公会宝藏】活动 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad TechLv
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv = 2
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307014 1 12800 29500 0 500 0 0 1800 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307014 1 12800 29500 0 501 0 0 1800 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307014 1 12800 29500 0 502 0 0 1800 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 30
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg 公会未研发“超能探测技术”科技，无法召唤【探路者】探测器
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00后会长若未开启，会员可自行开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv = 2
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307014 1 12800 29500 0 500 0 0 1800 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307014 1 12800 29500 0 501 0 0 1800 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307014 1 12800 29500 0 502 0 0 1800 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 30
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg 公会未研发“超能探测技术”科技，无法召唤【探路者】探测器
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会宝藏】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 200

		strvar Pos
		player_clan_pos_info 1 0 0 0 Pos
		DYN_ACTIVE_STATUS_SET 1 19 1
		
		add_sys_msg_start
		if Pos = 7
			add_sys_msg [$_ClanName$]公会副会长[$_player$]开启公会宝藏活动，搜寻蕴藏在公会城市底部神秘的宝藏。
		endif
		if Pos = 8
			add_sys_msg [$_ClanName$]公会会长[$_player$]开启公会宝藏活动，搜寻蕴藏在公会城市底部神秘的宝藏。
		endif
		if Pos != 7 and Pos != 8
			add_sys_msg [$_ClanName$]公会成员[$_player$]开启公会宝藏活动，搜寻蕴藏在公会城市底部神秘的宝藏。
		endif
		set_sys_msg_mode 1 0 0
		svr_msg_info
		
		
		;记录公会ID 用于公会频道广播
		strvar ClanID
		get_pworld_var 0 32 1 ClanID		
		GET_ID_PLAYER_INFO 0 18 ClanID
		set_pworld_var 0 32 @ClanID 1
	    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		add_sys_msg_start
		if Pos = 7		
			add_sys_msg 公会副会长【$_player$】开启了寻找公会宝藏活动！
		endif
		if Pos = 8
			add_sys_msg 公会会长【$_player$】开启了寻找公会宝藏活动！
		endif
		if Pos != 7 and Pos != 8
			add_sys_msg 公会成员【$_player$】开启了寻找公会宝藏活动！
		endif 
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @ClanID

endproc

proc 151

	strvar RetNum inum
	GET_PACK_SPACE 0 RetNum
	GET_ITEM_NUM_INPAK 5052101 1 inum
	if inum = 0
		if RetNum >= 1			
			
			ADD_BIND_ITEM_NUM_INPAK 5052101 1 1 0
		else
			add_sys_msg_start
			add_sys_msg 背包已满，无法领取物品！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已经有蒸汽挖掘器了，不可重复领取
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

;proc 152
;	strvar hp
;	GET_ID_PLAYER_INFO 0 2 hp
;;	if hp <= 0
;		add_sys_msg_start
;		add_sys_msg 死亡状态不可开启活动！
;		set_sys_msg_mode 1 0 0
;		msg_info
;	endif
;endproc
