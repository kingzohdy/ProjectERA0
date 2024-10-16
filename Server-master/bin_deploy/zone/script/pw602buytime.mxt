;公会守护枢纽活动    ,购买buff
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b枢纽日常的一些维护和修理都是我在维持，枢纽主要有一点点的异常情况我肯定会知道的。
	option 200 #公会枢纽短暂无敌
	option 300 #敌方袭击时间延后
	talk
endproc
proc 200
	choose 2 201 202 购买后公会枢纽30秒内处于无敌状态，需要消耗20金币，确定购买？
endproc
proc 201
	strvar sec
	get_time_sec sec
	;monmemid10,枢纽怪memid
	var 10 0
	get_pworld_var 0 32 1 10
	if 10 = 0
		add_sys_msg_start
		add_sys_msg 无法进行此操作  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	;cool13,无敌冷却时间
	var 13 0
	get_pworld_var 12 32 1 13
	if 13 > @sec
;		add_sys_msg_start
;		add_sys_msg $@13$:$@sec$
;		set_sys_msg_mode 3 0 0
;		map_msg_info
		add_sys_msg_start
		add_sys_msg 购买失败，公会枢纽无敌3分钟内只能购买一次  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
		
	DEC_MONEY_TEST 0 20 60 1
	if 1 != 0
		add_sys_msg_start
		add_sys_msg 金币不足，公会枢纽无法进入无敌状态  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	dec_money 0 20 60 1
	
	if 10 > 0
	;TARGET_ADD_STATUS   #目标类型 #memid #statusID #statusLV #statusTM #返回值
		TARGET_ADD_STATUS 3 @10 3170 1 30 1
		if 1 = 0
;			add_sys_msg_start
;			add_sys_msg $@13$:$@sec$
;			set_sys_msg_mode 3 0 0
;			map_msg_info
			add_sys_msg_start
			add_sys_msg 购买公会枢纽无敌成功，公会枢纽30秒内处于无敌状态 
			set_sys_msg_mode 4 0 0
			map_msg_info
			add sec 180
			;cool13,无敌冷却时间
			var 13 @sec
			set_pworld_var 12 32 @13 1
		endif
	endif	
endproc
proc 202
;	call 203
endproc
proc 203
	;monmemid10,枢纽怪memid
	var 10 0
	get_pworld_var 0 32 1 10
	if 10 = 0
		return
	endif
	TARGET_ADD_STATUS 3 @10 3170 1 30 1
endproc
proc 300
	choose 2 301 302 购买后能将敌方袭击时间延后20秒，需要消耗10金币，确定购买？
endproc
proc 301
	DEC_MONEY_TEST 0 10 60 1
	if 1 != 0
		add_sys_msg_start
		add_sys_msg 金币不足，敌方袭击时间无法延后  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	is_pworld_over 1
	if 1 = 1
		add_sys_msg_start
		add_sys_msg 无法进行此操作  
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	dec_money 0 10 60 1
	GET_PWORLD_wave_MON_COUNT_DOWN 10
	add 10 20
	SET_PWORLD_wave_MON_COUNT_DOWN 1 @10
endproc
proc 302
endproc
