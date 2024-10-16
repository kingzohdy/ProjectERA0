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
		SET_MON_TASK_STAT 30001 2 1
endproc

proc 3
;怪物巡逻结束事件
	STRVAR PatrolID Mon_Ret 
	
	STRVAR NumRand_1 NumRand_2 NumRand_3	
	
	GET_MON_INFO 1 40 41 42 43 44 45 46
	;得到怪物所在地图/信息
	GET_MON_PATROL_ID @41 Mon_Ret PatrolID
	;得到怪物路径ID
	if Mon_Ret = 0
		if PatrolID = 18 or PatrolID = 29 or PatrolID = 30
				SET_MON_TASK_STAT 30001 1 1
				DEL_MON 1
		endif
		if PatrolID = 19 or PatrolID = 31 or PatrolID = 32
				SET_MON_TASK_STAT 30001 1 1
				DEL_MON 1
		endif
		if PatrolID = 20 or PatrolID = 33 or PatrolID = 34
				SET_MON_TASK_STAT 30001 1 1
				DEL_MON 1
		endif
		
		if PatrolID = 13 
			rand NumRand_1 3
			add NumRand_1 100
			switch NumRand_1
				case 100
					MON_CHG_MAP 2100 4110 3912 18 20 1
				endcase
				case 101
					MON_CHG_MAP 2100 4110 3912 29 20 1
				endcase
				case 102
					MON_CHG_MAP 2100 4110 3912 30 20 1
				endcase
				
			endswitch
		endif
		if PatrolID = 14 
			rand NumRand_2 3
			add NumRand_2 100
			switch NumRand_2
				case 100
					MON_CHG_MAP 2200 22129 3886 19 20 1
				endcase 
				case 101
					MON_CHG_MAP 2200 22129 3886 31 20 1
				endcase
				case 102
					MON_CHG_MAP 2200 22129 3886 32 20 1
				endcase
			
			endswitch
		
		endif
		
		if PatrolID = 15 
			rand NumRand_3 3
			add NumRand_3 100
			switch NumRand_3
				case 100
					MON_CHG_MAP 2300 16190 20698 20 20 1
				endcase 
				case 101
					MON_CHG_MAP 2300 16190 20698 33 20 1
				endcase
				case 102
					MON_CHG_MAP 2300 16190 20698 34 20 1
				endcase
			
			endswitch
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
		MON_TASK_AREA_CHECK 30001 3000 300 200		
		;检查有效玩家
;	endif
			
;	add 11 1
;	SET_MON_VAR 4 8 @11 1	
;	if 11 = 7 
;		MON_TASK_AREA_CHECK 30001 3000 300 200 
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
			MON_CALL_ENEMY_MON 200024 2 100 1 @MonX @MonY 1000 0 0 0 30 1 0
			MON_CALL_ENEMY_MON 200025 2 100 1 @MonX @MonY 1000 0 0 0 30 1 0
			var 13 0
			SET_MON_VAR 1 8 @13 1		
		endif
	endif

	endif

endproc
