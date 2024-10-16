proc 1
	
	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b支撑着整个世界运转的神源，不仅是能源的输送终端，经魔法师的长期研究发现，神源的力量远不如此。
	add_talk #b#b受神源影响的人，大脑对事物的接受与吸收量更是常人的两倍！因此被大量冒险家利用来提升冒险的进度。
	add_talk #b#b#c186506可领取双倍经验状态，增加100%杀怪经验#n
	call 200
	
	OPTION 101 #领取0.5小时双倍时间 
	OPTION 102 #领取1小时双倍时间
	OPTION 103 #领取2小时双倍时间
	OPTION 104 #查看本周剩余双倍时间 
	talk
endproc

proc 200
	strvar Ret1 Ret2 Ftime Times
	get_role_var 1 32 Ret1 Ftime
	if Ret1 = -1
		return
	endif
	IS_SAME_WEEK @Ftime Ret2
	;是否同一周
	if @Ret2 = -1
		var Times 0
		set_role_var 5 8 @Times Ret2
	endif
endproc

proc 101
	
	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #角色实例ID（0表示自己） #AttrFlag  #返回值
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times < 14
;			if Ret4 = -1
			;是否有双倍状态
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				add Times 1
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 1800 Ret3
				
				add_sys_msg_start
				add_sys_msg 您成功领取0.5小时双倍经验状态！
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg 您已有双倍经验状态，请稍后领取！
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			add_sys_msg_start
			add_sys_msg 本周您双倍经验时间已用完，请等待下周更新！
			set_sys_msg_mode 4 0 0
			msg_info
		endif

	else
		return
	endif
endproc

proc 102
	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #角色实例ID（0表示自己） #AttrFlag  #返回值
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times <= 12
;			if Ret4 = -1
			;是否有双倍状态
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				
				add Times 2
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 3600 Ret3
				
				add_sys_msg_start
				add_sys_msg 您成功领取1小时双倍经验状态！
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg 您已有双倍经验状态，请稍后领取！
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			if Times = 14
				add_sys_msg_start
				add_sys_msg 本周您双倍经验时间已用完，请等待下周更新！
				set_sys_msg_mode 4 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg 本周您双倍经验时间不足1小时，无法领取！
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		endif
	else
		return
	endif
endproc

proc 103

	call 200
	strvar Ret1 Ret2 Ret3 Ret4
	strvar Ftime Times MemID
	;GET_ID_PLAYER_INFO #角色实例ID（0表示自己） #AttrFlag  #返回值
	GET_ID_PLAYER_INFO 0 1 MemID
	HAS_STATUS 1052 Ret4
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times <= 10
;			if Ret4 = -1
			;是否有双倍状态
				if Times = 0
					GET_TIME_SEC Ftime
					set_role_var 1 32 @Ftime Ret1
				endif
				
				add Times 4
				set_role_var 5 8 @Times Ret2
				TARGET_ADD_STATUS 1 @MemID 1052 1 7200 Ret3
				
				add_sys_msg_start
				add_sys_msg 您成功领取2小时双倍经验状态！
				set_sys_msg_mode 3 0 0
				msg_info
;			else
;				add_sys_msg_start
;				add_sys_msg 您已有双倍经验状态，请稍后领取！
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
		else
			if Times = 14
				add_sys_msg_start
				add_sys_msg 本周您双倍经验时间已用完，请等待下周更新！
				set_sys_msg_mode 4 0 0
				msg_info
			else
				add_sys_msg_start
				add_sys_msg 本周您双倍经验时间不足2小时，无法领取！
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		endif
	else
		return
	endif
endproc

proc 104

	call 200
	strvar Ret1 Ret2 Ret3
	strvar Ftime Times Stop Ltime
	get_role_var 5 8 Ret2 Times
	if Ret2 = 0
		if Times = 0 or @Times = 2 or @Times = 4 or @Times = 6 or @Times = 8 or @Times = 10 or @Times = 12 or @Times = 14
			var Ltime 14
			sub Ltime @Times
			dev Ltime 2
					add_sys_msg_start
					add_sys_msg 您本周双倍经验时间剩余$@Ltime$小时！
					set_sys_msg_mode 3 0 0
					msg_info
		endif
		
		if Times = 1 or @Times = 3 or @Times = 5 or @Times = 7 or @Times = 9 or @Times = 11 or @Times = 13
			var Ltime 14
			sub Ltime @Times
			dev Ltime 2
					add_sys_msg_start
					add_sys_msg 您本周双倍经验时间剩余$@Ltime$.5小时！
					set_sys_msg_mode 3 0 0
					msg_info
		endif
	else
		return
	endif
endproc
