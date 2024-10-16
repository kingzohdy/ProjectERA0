;黑暗物质被击打掉钱
;黑暗物质被打
proc 1
	SET_MON_VAR 0 8 -1 1
	
	strvar x y
	GET_MON_INFO 0 200029 0 0 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\700100_4.ent")
	
endproc

proc 2
	strvar ret
	GET_EVENT_STAT ret 56
	GET_MON_INFO 0 0 0 20 0 0 0 0
	if ret = 1 and 20 = 0
		GET_MON_INFO 0 0 0 0 0 0 30 40
		NEW_MON 352114 1 @30 @40 500 0 0 0 0 

	endif
endproc 


proc 10
	effect 0 3 playMontion(%d,"114210_4")
	strvar rnd
	RAND rnd 30
	switch @rnd
		case 0
			mon_speech 0 0 2 1 我记着你了！有一天我会报仇的！
		endcase
		case 1
			mon_speech 0 0 2 1 还我的钱！还我的钱！
		endcase
		case 2
			mon_speech 0 0 2 1 杀了一个我！还有千千万万个我！
		endcase
	endswitch
	
	SET_MON_VAR 0 8 0 1
	get_mon_mapdefid 0 10
	strvar money
	var money 0
	if @0 = -1
		return
	else
		switch @10
			case 1000
				var money 30
			endcase
			case 2202
				var money 24
			endcase
			case 2200
				var money 30
			endcase
			case 2300
				var money 36
			endcase
			case 2400
				var money 48
			endcase
			case 2500
				var money 60
			endcase
			case 2104
				var money 72
			endcase
			case 2201
				var money 84
			endcase
			case 3404
				var money 90
			endcase
			case 3303
				var money 36
			endcase
			case 3300
				var money 42
			endcase
			case 3302
				var money 48
			endcase
			case 3400
				var money 54
			endcase
			case 3500
				var money 66
			endcase
			case 3100
				var money 78
			endcase
			case 3200
				var money 96
			endcase
			case 3304
				var money 102
			endcase
			case 3110
				var money 96
			endcase
			case 3210
				var money 102
			endcase
		endswitch
	endif
	if @money > 0
		darkmatter_dropmoney 0 @money
	endif
endproc

;怪物定时，1秒一次
proc 11
	strvar NUM
	var NUM 3
	GET_MON_VAR 0 8 0 10

	if @10 = -1
		return
	elseif @10 = 0
		;打完后下一次才开始逃跑掉钱
		ADD @10 1
		SET_MON_VAR 0 8 @10 1
	elseif @10 <= @NUM
		;逃跑掉钱算法[（Map告诉程序，哪个地图掉多少银币）/N次数）在概率 0.8~1.2 中取值]
		strvar money GL CPNUM
		var money 0
		get_mon_mapdefid 0 11
		switch @11
			case 1000
				var money 30
			endcase
			case 2202
				var money 24
			endcase
			case 2200
				var money 30
			endcase
			case 2300
				var money 36
			endcase
			case 2400
				var money 48
			endcase
			case 2500
				var money 60
			endcase
			case 2104
				var money 72
			endcase
			case 2201
				var money 84
			endcase
			case 3404
				var money 90
			endcase
			case 3303
				var money 36
			endcase
			case 3300
				var money 42
			endcase
			case 3302
				var money 48
			endcase
			case 3400
				var money 54
			endcase
			case 3500
				var money 66
			endcase
			case 3100
				var money 78
			endcase
			case 3200
				var money 96
			endcase
			case 3304
				var money 102
			endcase
			case 3110
				var money 96
			endcase
			case 3210
				var money 102
			endcase
		endswitch
		
		if @money <= 0
			return
		endif
		
		RAND GL 8
		add GL 3
		
		MUL money @GL
		VAR CPNUM @NUM
		MUL CPNUM 10
		DEV money @CPNUM
		DARKMATTER_FLEE_DROPMONEY 0 @money
		if @10 < @NUM
			ADD @10 1
			SET_MON_VAR 0 8 @10 1
		else
			SET_MON_VAR 0 8 -1 1
		endif
	else
		return
	endif
endproc
