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
		SET_MON_TASK_STAT 30014 2 1
endproc

proc 3
;怪物巡逻结束事件
	STRVAR PatrolID Mon_Ret NumRand_1
	
	GET_MON_INFO 1 40 41 42 43 44 45 46
	;得到怪物所在地图/信息
	GET_MON_PATROL_ID @41 Mon_Ret PatrolID
	;得到怪物路径ID
	
	if Mon_Ret = 0
		if PatrolID = 30 or PatrolID = 19 or PatrolID = 31 or PatrolID = 32
				SET_MON_TASK_STAT 30014 1 1
				DESTROY_MON 1
		endif

		if PatrolID = 29
			rand NumRand_1 4
			add NumRand_1 100
			switch NumRand_1
				case 100
					MON_CHG_MAP 2201 32800 4400 30 20 1
				endcase
				case 101
					MON_CHG_MAP 2201 32800 4400 19 20 1
				endcase
				case 102
					MON_CHG_MAP 2201 32800 4400 31 20 1
				endcase
				case 103
					MON_CHG_MAP 2201 32800 4400 169 20 1
				endcase
			endswitch
		endif
		
		if PatrolID = 13
				MON_CHG_MAP 2100 4200 4000 18 20 1	
		endif
		if PatrolID = 18
				MON_CHG_MAP 2200 22300 3500 29 20 1	
		endif
		
		if PatrolID = 169
				MON_CHG_MAP 3200 6200 23400 32 20 1	
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
		MON_TASK_AREA_CHECK 30014 3000 300 200		
		;检查有效玩家
;	endif
			
;	add 11 1
;	SET_MON_VAR 4 8 @11 1	
;	if 11 = 7 
;		MON_TASK_AREA_CHECK 30004 3000 300 200 
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
				strvar Rad
				rand Rad 2
				if Rad = 0
					TARGET_ADD_STATUS 3 @30 1009 2 20 1
					MON_CALL_ENEMY_MON 200026 2 75 2 @MonX @MonY 1000 0 0 0 60 1 0
					MON_CALL_ENEMY_MON 200027 2 75 2 @MonX @MonY 1000 0 0 0 60 1 0
				endif
				var 13 0
				SET_MON_VAR 1 8 @13 1		
				
			endif
		endif

	endif

endproc
