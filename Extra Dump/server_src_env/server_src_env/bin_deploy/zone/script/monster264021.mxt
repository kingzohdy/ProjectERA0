;运镖
 
proc 1
;怪物刷出
	var 10 0
	SET_MON_VAR 0 8 @10 1
	var 11 0
	SET_MON_VAR 4 8 @11 1
	var 12 0
	SET_MON_VAR 7 8 @12 1	
	var 13 0
	SET_MON_VAR 1 8 @13 1		
	var 200 100
	STRVAR MonX MonY MonID

	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
	TARGET_ADD_STATUS 3 @30 1009 2 10 1
endproc
proc 2
;怪物死亡
		SET_MON_TASK_STAT 30003 2 1
endproc

proc 3
;怪物巡逻结束事件
	STRVAR PatrolID Mon_Ret
	
	GET_MON_INFO 1 40 41 42 43 44 45 46
	;得到怪物所在地图/信息
	GET_MON_PATROL_ID @41 Mon_Ret PatrolID
	;得到怪物路径ID
	
	if Mon_Ret = 0
		if PatrolID = 22 or PatrolID = 25 or PatrolID = 28
				SET_MON_TASK_STAT 30003 1 1
				DEL_MON 1
		endif

		if PatrolID = 15 
				MON_CHG_MAP 2300 4110 3912 21 20 1	
		endif
		if PatrolID = 21 
				MON_CHG_MAP 3300 1005 7374 22 20 1	
		endif
		if PatrolID = 16
				MON_CHG_MAP 2400 3320 3550 24 20 1	
		endif
		if PatrolID = 24
				MON_CHG_MAP 3400 2855 3153 25 20 1	
		endif
		if PatrolID = 17 
				MON_CHG_MAP 2500 31067 22561 27 20 1			
		endif
		if PatrolID = 27
				MON_CHG_MAP 3500 5327 12796 28 20 1			
		endif

	endif
endproc

proc 4
;3秒执行一次


;	GET_MON_VAR 0 8 1 10
;	GET_MON_VAR 4 8 1 11	
	GET_MON_VAR 7 8 1 12	
	GET_MON_VAR 1 8 1 13	

	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
		;获得玩家等级
	
;	if 11 = 0 
		MON_TASK_AREA_CHECK 30003 3000 300 200		
		;检查有效玩家
;	endif
			
;	add 11 1
;	SET_MON_VAR 4 8 @11 1	
;	if 11 = 7 
;		MON_TASK_AREA_CHECK 30003 3000 300 200 
;		;检查有效玩家
;		SET_MON_VAR 4 8 0 1	
;	endif
	
	if 200 = 0
	;失败后
			
			GET_MON_VAR 7 8 1 12
			add 12 1
			SET_MON_VAR 7 8 @12 1			
			if 12 = 1
				DEL_MON 1			
			endif
	else

	
	if 32 != 1000

		add 13 1
		SET_MON_VAR 1 8 @13 1	
		
		if 13 = 20 
			;60秒时让镖车停下30秒召唤怪物
			TARGET_ADD_STATUS 3 @30 1009 2 20 1
			MON_CALL_ENEMY_MON 200026 3 80 1 @MonX @MonY 1000 0 0 0 30 1 0
			MON_CALL_ENEMY_MON 200027 3 80 1 @MonX @MonY 1000 0 0 0 30 1 0
			var 13 0
			SET_MON_VAR 1 8 @13 1		
		endif
	endif

	endif

endproc
