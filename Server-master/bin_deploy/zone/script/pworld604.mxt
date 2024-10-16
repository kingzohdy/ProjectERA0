;末世巅峰副本

;创建副本事件
PROC 1
	;天灾或各特殊事件
	set_pworld_var 0 32 0 0
	;1分钟过后
	set_pworld_var 4 32 0 0
	;增加攻击点
	set_pworld_var 8 32 0 0
	;触发哪一个天灾人祸
	set_pworld_var 12 32 0 0 
	;加攻击点，当前处于哪个阶段，玩家重新登录的时候，给加攻击点
	set_pworld_var 16 32 0 0 
	;随机到了
	set_pworld_var 20 32 0 0
ENDPROC

;关闭副本事件
proc 2
endproc

;副本定时事件（每2秒触发一次）
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		get_pworld_var 4 32 1 11
		if @11 >= 30
			strvar flag rVar rNum
			var flag 0
			get_pworld_var 0 32 1 15
			;1分钟后，每16s 进行随机，并提示，每26s进行随机天灾
			if @15 = 8
				RAND rVar 100
				;75%的几率
				if @rVar > 24
					set_pworld_var 20 32 1 0
					RAND rNum 12
					set_pworld_var 12 32 @rNum 0
					call 100
				endif
				set_pworld_var 0 32 9 0
			elseif  @15 = 14
				get_pworld_var 20 32 1 flag
				if @flag = 1
					call 101
				endif
				set_pworld_var 20 32 0 0
				set_pworld_var 0 32 0 0
			else
				add 15 1
				set_pworld_var 0 32 @15 0
			endif
			
			;1分钟后  30s发生
			get_pworld_var 8 32 1 12
			if @12 = 15
				;攻击增加5000点
				call 110
				set_pworld_var 16 32 1 0 
				set_pworld_var 8 32 16 0 
			elseif @12 = 31
				;攻击增加15000点
				call 111
				set_pworld_var 16 32 2 0 
				set_pworld_var 8 32 32 0 
			elseif @12 = 46
				;攻击增加30000点
				call 112
				set_pworld_var 16 32 3 0 
				set_pworld_var 8 32 47 0 
			elseif @12 = 61
				;攻击增加60000点
				call 113
				set_pworld_var 16 32 4 0 
				set_pworld_var 8 32 62 0 
			elseif @12 = 76
				set_pworld_var 16 32 0 0
			else
				add 12 1
				set_pworld_var 8 32 @12 0
			endif
		else
			add 11 1
			set_pworld_var 4 32 @11 1
		endif
	endif	
endproc

;副本中角色进入事件
proc 5	
	strvar memvar
	get_player_id 10 memvar
	if 10 = -1
		return 
	endif	
	
	get_pworld_var 16 32 1 90
	if @90 = 1
		TARGET_ADD_STATUS  1 @memvar 1306 1 60 11
	elseif @90 = 2
		TARGET_ADD_STATUS  1 @memvar 1306 2 60 11
	elseif @90 = 3
		TARGET_ADD_STATUS  1 @memvar 1306 3 60 11
	elseif @90 = 4
		TARGET_ADD_STATUS  1 @memvar 1306 4 60 11
	else
	endif
endproc

;提示
proc 100
	strvar rVar 
	get_pworld_var 12 32 1 rVar
	if @rVar < 6
		add_sys_msg_start
		add_sys_msg 战神祝福将在10S后洒落在沙场上，请做好争夺准备！
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	else 
		add_sys_msg_start
		add_sys_msg 末世诅咒即将降临，请做好躲避准备！
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	endif
endproc

;有几率天灾人祸或各种特殊事件
proc 101
	strvar rNum PosX PosY
	strvar RandPos
	get_pworld_var 12 32 1 rNum
	
	RAND RandPos 4
	if @RandPos = 0
		var PosX 7000
		var PosY 6500
	elseif @RandPos = 1
		var PosX 7000
		var PosY 5700
	elseif @RandPos = 2
		var PosX 6200
		var PosY 5700
	else
		var PosX 6200
		var PosY 6500
	endif
	
	if @rNum = 0
		NEW_LIFE_MON 261401 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 1
		NEW_LIFE_MON 261402 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 2
		NEW_LIFE_MON 261403 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 3
		NEW_LIFE_MON 261404 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 4
		NEW_LIFE_MON 261405 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 5
		NEW_LIFE_MON 261406 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 6
		NEW_LIFE_MON 261407 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 7
		NEW_LIFE_MON 261408 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 8
		NEW_LIFE_MON 261409 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 9
		NEW_LIFE_MON 261410 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 10
		NEW_LIFE_MON 261411 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 11
		NEW_LIFE_MON 261412 1 @PosX @PosY 800 0 0 0 10 0 0
	else
	endif
endproc

;增加攻击点
proc 110
	add_sys_msg_start
	add_sys_msg 在长时间的战斗中，全身战斗细胞得到激活，攻击获得小幅提升！
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO

	;给巅峰副本里面所有人加状态
	DIANFENG_ADD_STATUS 1306 1 60 90
endproc

proc 111
	add_sys_msg_start
	add_sys_msg 在长时间的战斗中，全身战斗细胞得到激活，攻击获得较高的提升！
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 2 60 90
endproc

proc 112
	add_sys_msg_start
	add_sys_msg 在长时间的战斗中，全身战斗细胞得到激活，攻击获得大幅提升！
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 3 60 90
endproc

proc 113
	add_sys_msg_start
	add_sys_msg 在长时间的战斗中，全身战斗细胞得到激活，攻击获得成倍的提升！
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 4 300 90
endproc
