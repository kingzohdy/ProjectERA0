;天降宏福活动    ,天降宏福活动
proc 1
	;begin10,开始时间
	var 10 0
	set_event_var 0 32 @10 1
	;time11,持续时间
	var 11 1200
	set_event_var 4 32 @11 1
	;count12,刷怪计数
	var 12 0
	set_event_var 8 32 @12 1
	;status13,状态id
	var 13 0
	set_event_var 12 32 @13 1
	;level14,状态等级
	var 14 0
	set_event_var 16 32 @14 1
	;monid15,怪物id
	var 15 0
	set_event_var 20 32 @15 1
	;mapid16,地图id
	var 16 0
	set_event_var 24 32 @16 1
	;dyn17,动态参数
	var 17 0
	set_event_var 28 32 @17 1
	;recordbegin18,记录的开始时间
	var 18 0
	set_event_var 32 32 @18 1
	;refresh19,刷怪间隔时间
	var 19 14400
	set_event_var 36 32 @19 1
	;pretip20,提前5分钟提示
	var 20 0
	set_event_var 40 32 @20 1
	strvar sec 
	get_time_sec sec
	;refresh19,刷怪间隔时间
	var 19 0
	get_event_var 36 32 1 19
	mod sec 86400
	dev sec @19
		
	;count12,刷怪计数
	var 12 @sec
	set_event_var 8 32 @12 1
endproc
proc 2
	
endproc
;tick,10秒一次
proc 3	
	is_lucky_closed 1 2
	if 2 = 0
		return
	endif
	;recordbegin18,记录的开始时间
	var 18 0
	get_event_var 32 32 1 18
	strvar sec 
	get_time_sec sec
	if 18 > 0
		;time11,持续时间
		var 11 0
		get_event_var 4 32 1 11
		add 18 @11
		
		
		var 11 @18
		sub 18 @sec
		if 18 > 0
			call 302
			return
		endif
	endif
	;count12,刷怪计数
	var 12 0
	get_event_var 8 32 1 12
	;refresh19,刷怪间隔时间
	var 19 0
	get_event_var 36 32 1 19
	mod sec 86400
	dev sec @19
	if 12 > @sec
		return
	endif
	;begin10,开始时间
	var 10 0
	get_event_var 0 32 1 10
	if 10 > 0
		call 99
	else
		call 98
		call 99
	endif
endproc
;玩家进入地图
proc 6
	is_lucky_closed 1 2
	if 2 = 1
		return
	endif
		
	get_player_mapinfo 1 2 0 0 0
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	if 16 = @2
		is_player_lucky_attendee 1 3
		if 3 != 1
			strvar sec 
			get_time_sec sec
			;recordbegin18,记录的开始时间
			var 18 0
			get_event_var 32 32 1 18
			sub sec @18
			;time11,持续时间
			var 11 0
			get_event_var 4 32 1 11
			sub 11 @sec
			choose_with_timer 1 90 91 @11 天降横福活动在本地图开始,确认参加?
		endif
	endif
	
endproc
proc 88
	;begin10,开始时间
	var 10 0
	get_event_var 0 32 1 10
	;time11,持续时间
	var 11 0
	get_event_var 4 32 1 11
	;count12,刷怪计数
	var 12 0
	get_event_var 8 32 1 12
	;status13,状态id
	var 13 0
	get_event_var 12 32 1 13
	;level14,状态等级
	var 14 0
	get_event_var 16 32 1 14
	;monid15,怪物id
	var 15 0
	get_event_var 20 32 1 15
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	;dyn17,动态参数
	var 17 0
	get_event_var 28 32 1 17
	;recordbegin18,记录的开始时间
	var 18 0
	get_event_var 32 32 1 18
	add_sys_msg_start
	add_sys_msg  开始时间:$@10$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  持续时间:$@11$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  刷怪计数:$@12$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  状态id:$@13$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  状态等级:$@14$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  怪物id:$@15$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  地图id:$@16$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  动态参数:$@17$
	set_sys_msg_mode 1 0 0
	map_msg_info
	add_sys_msg_start
	add_sys_msg  记录的开始时间:$@18$
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
proc 90
	get_player_mapinfo 1 2 0 0 0
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	if 16 != @2
		return
	endif
	strvar sec 
	get_time_sec sec
	;recordbegin18,记录的开始时间
	var 18 0
	get_event_var 32 32 1 18
	sub sec @18
	;time11,持续时间
	var 11 0
	get_event_var 4 32 1 11
	sub 11 @sec
	if 11 < 1
		return
	endif
	GET_PLAYER_ID 1 4
	target_add_status 1 @4 3110 5 @11 1
endproc
proc 91
endproc
proc 98
	strvar sec
	get_time_sec sec
	;refresh19,刷怪间隔时间
	var 19 0
	get_event_var 36 32 1 19
	mod sec 86400
	var 2 @sec
	dev 2 @19
	mod sec @19
	;time11,持续时间
	var 11 0
	get_event_var 4 32 1 11
	var 3 @19
	sub 3 @11
	if sec >= @3
		return
	endif
	var 4 @19
	sub 4 @11
	sub 4 20
	
	rand sec @4
	
	mul 2 @19
	add sec @2
	;begin10,开始时间
	var 10 @sec
	set_event_var 0 32 @10 1
	;recordbegin18,记录的开始时间
	var 18 0
	set_event_var 32 32 @18 1
	strvar r p m map mon level
;	rand p 18
;	var m @p
;	add p 100
;	call @p
	GET_RAND_LUKY_MAP 1 map mon level
	;mapid16,地图id
	var 16 @map
	set_event_var 24 32 @16 1
	;monid15,怪物id
	var 15 @mon
	set_event_var 20 32 @15 1
	;dyn17,动态参数
	var 17 @level
	set_event_var 28 32 @17 1
	;pretip20,提前5分钟提示
	var 20 0
	set_event_var 40 32 @20 1
	
	rand r 6
	
	if r > 5
		var p 3
	elseif r > 2
		var p 2
	elseif r > 0
		var p 1
	else
		var p 0
	endif
;	if m < 12
;		if r < 4
;			var p @r
;		else
;			var p 3
;		endif
;	endif
	add p 200
	call @p
endproc
proc 99
	strvar sec tipsec
	get_time_sec sec
	mod sec 86400
	;begin10,开始时间
	var 10 0
	get_event_var 0 32 1 10
	if 10 = 0
		return
	endif
	var tipsec @10
	sub tipsec 300
	if sec < @tipsec
		return
	endif
	
	if sec > @tipsec and sec < @10
		call 100
		return
	endif
;	if sec < @10
;		return
;	endif
;	mod sec 86400
;	mod sec 14400
	
;	if sec >= 9600
;		return
;	endif
	
	call 300
endproc
proc 100
	;pretip20,提前5分钟提示
	var 20 0
	get_event_var 40 32 1 20
	if 20 = 0
		add_sys_msg_start
		add_sys_msg  【$_LuckyMapName$】异相突生，预计将于五分钟后星坠大地，天降横福！ 
		set_sys_msg_mode 6 0 0
		svr_msg_info
		var 20 1
		;pretip20,提前5分钟提示
		
		set_event_var 40 32 @20 1
	endif
endproc
;2300,流金沙漠,35,206405
;proc 100
;	;mapid16,地图id
;	var 16 2300
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206405
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 35
;	set_event_var 28 32 @17 1
;endproc
;;3200,牢城,77,206417
;proc 100
;	;mapid16,地图id
;	var 16 3200
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206417
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 77
;	set_event_var 28 32 @17 1
;endproc
;
;;2400,落日荒野,45,206406
;proc 101
;	;mapid16,地图id
;	var 16 2400
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206406
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 45
;	set_event_var 28 32 @17 1
;endproc
;
;;2500,越雾雨林,54,206407
;proc 102
;	;mapid16,地图id
;	var 16 2500
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206407
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 54
;	set_event_var 28 32 @17 1
;endproc
;
;;3300,阿努比斯神殿,37,206411
;proc 103
;	;mapid16,地图id
;	var 16 3300
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206411
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 37
;	set_event_var 28 32 @17 1
;endproc
;
;;3302,阿努比斯祭坛,42,206412
;proc 104
;	;mapid16,地图id
;	var 16 3302
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206412
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 42
;	set_event_var 28 32 @17 1
;endproc
;
;;3303,阿努比斯废墟,38,206413
;proc 105
;	;mapid16,地图id
;	var 16 3303
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206413
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 38
;	set_event_var 28 32 @17 1
;endproc
;
;;3304,法老陵,83,206418
;proc 106
;	;mapid16,地图id
;	var 16 3304
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206418
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 83
;	set_event_var 28 32 @17 1
;endproc
;
;;2400,落日荒野,45,206406
;proc 107
;	;mapid16,地图id
;	var 16 2400
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206406
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 45
;	set_event_var 28 32 @17 1
;endproc
;
;;2500,越雾雨林,54,206407
;proc 108
;	;mapid16,地图id
;	var 16 2500
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206407
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 54
;	set_event_var 28 32 @17 1
;endproc
;
;;3300,阿努比斯神殿,37,206411
;proc 109
;	;mapid16,地图id
;	var 16 3300
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206411
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 37
;	set_event_var 28 32 @17 1
;endproc
;
;;3302,阿努比斯祭坛,42,206412
;proc 110
;	;mapid16,地图id
;	var 16 3302
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206412
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 42
;	set_event_var 28 32 @17 1
;endproc
;
;;3303,阿努比斯废墟,38,206413
;proc 111
;	;mapid16,地图id
;	var 16 3303
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206413
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 38
;	set_event_var 28 32 @17 1
;endproc
;
;;2104,荫川林地,63,206408
;proc 112
;	;mapid16,地图id
;	var 16 2104
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206408
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 63
;	set_event_var 28 32 @17 1
;endproc
;
;;2201,风回裂谷,73,206409
;proc 113
;	;mapid16,地图id
;	var 16 2201
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206409
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 73
;	set_event_var 28 32 @17 1
;endproc
;
;;3404,鸣沙陷地,78,206410
;proc 114
;	;mapid16,地图id
;	var 16 3404
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206410
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 78
;	set_event_var 28 32 @17 1
;endproc
;
;
;;3400,十三号矿洞,50,206414
;proc 115
;	;mapid16,地图id
;	var 16 3400
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206414
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 50
;	set_event_var 28 32 @17 1
;endproc
;
;;3500,越雾甬道,58,206415
;proc 116
;	;mapid16,地图id
;	var 16 3500
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206415
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 58
;	set_event_var 28 32 @17 1
;endproc
;
;;3100,普罗圣地,66,206416
;proc 117
;	;mapid16,地图id
;	var 16 3100
;	set_event_var 24 32 @16 1
;	;monid15,怪物id
;	var 15 206416
;	set_event_var 20 32 @15 1
;	;dyn17,动态参数
;	var 17 66
;	set_event_var 28 32 @17 1
;endproc
;3110,1,洪福齐天
proc 200
	;status13,状态id
	var 13 3110
	set_event_var 12 32 @13 1
	;level14,状态等级
	var 14 1
	set_event_var 16 32 @14 1
endproc
;3110,2,财源广进
proc 201
	;status13,状态id
	var 13 3110
	set_event_var 12 32 @13 1
	;level14,状态等级
	var 14 2
	set_event_var 16 32 @14 1
endproc
;3110,3,天佑英才
proc 202
	;status13,状态id
	var 13 3110
	set_event_var 12 32 @13 1
	;level14,状态等级
	var 14 3
	set_event_var 16 32 @14 1
endproc
;3110,4,有求必应
proc 203
	;status13,状态id
	var 13 3110
	set_event_var 12 32 @13 1
	;level14,状态等级
	var 14 4
	set_event_var 16 32 @14 1
endproc
proc 300
	strvar x y
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	get_map_rand_mon_pos 1 @16 x y
	;monid15,怪物id
	var 15 0
	get_event_var 20 32 1 15
;	NEW_MON @15 1  0 0 0 0 2
	
	;time11,持续时间
	var 11 0
	get_event_var 4 32 1 11
	;status13,状态id
	var 13 0
	get_event_var 12 32 1 13
	;level14,状态等级
	var 14 0
	get_event_var 16 32 1 14
	;dyn17,动态参数
	var 17 0
	get_event_var 28 32 1 17
	
	;金卷时间特殊
;	if 14 = 1
;		var 11 600
;	endif
	NEW_MAP_MON  @15 1 @16 @x @y 0 0 0 0 @11 3 2
	;金卷特殊
	if 14 = 1
		TARGET_ADD_STATUS 3 @2 @13 @14 @11 1
	else
		TARGET_ADD_STATUS_dyn @17 3 @2 @13 @14 @11 1
	endif
	strvar ret
	var ret @1
	;count12,刷怪计数
	var 12 0
	get_event_var 8 32 1 12
	if ret = 0
		add 12 1
		;count12,刷怪计数
		
		set_event_var 8 32 @12 1
		;begin10,开始时间
		var 10 0
		set_event_var 0 32 @10 1
		set_lucky_open 1
		strvar sec
		get_time_sec sec
		;recordbegin18,记录的开始时间
		var 18 @sec
		set_event_var 32 32 @18 1
		
		call 301
	endif
endproc
proc 301
	strvar  ret it1 it2
	var ret 0
	var it1 0
	var it2 0
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	while ret = 0
		for_each_map_obj ret @16 1 it1 it2
		if ret = 0
			call 6
			;
		endif			
	endwhile
endproc
proc 302
	strvar x y
	;mapid16,地图id
	var 16 0
	get_event_var 24 32 1 16
	get_map_rand_mon_pos 1 @16 x y
	;monid15,怪物id
	var 15 0
	get_event_var 20 32 1 15
;	NEW_MON @15 1  0 0 0 0 2
	
;	;time11,持续时间
;	var 11 0
;	get_event_var 4 32 1 11
	;status13,状态id
	var 13 0
	get_event_var 12 32 1 13
	;level14,状态等级
	var 14 0
	get_event_var 16 32 1 14
	;dyn17,动态参数
	var 17 0
	get_event_var 28 32 1 17
	
	;recordbegin18,记录的开始时间
	var 18 0
	get_event_var 32 32 1 18
	;time11,持续时间
	var 11 0
	get_event_var 4 32 1 11
	add 11 @18
	strvar sec 
	get_time_sec sec
	sub 11 @sec
	if 11 <= 0		
		return
	endif
	NEW_MAP_MON  @15 1 @16 @x @y 0 0 0 0 @11 3 2
		;金卷特殊
	if 14 = 1
		TARGET_ADD_STATUS 3 @2 @13 @14 @11 1
	else
		TARGET_ADD_STATUS_dyn @17 3 @2 @13 @14 @11 1
	endif
	strvar ret
	var ret @1	
	if ret = 0		
		set_lucky_open 1	
		call 301
	endif
endproc
