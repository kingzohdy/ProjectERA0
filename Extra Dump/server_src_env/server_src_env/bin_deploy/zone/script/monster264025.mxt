;第二版运镖 40级队伍安全
 
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
	STRVAR OutMonX OutMonY OutRet

	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
	TARGET_ADD_STATUS 3 @30 1009 2 10 1
endproc
proc 2
;怪物死亡
		SET_MON_TASK_STAT 30004 2 1
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
		if PatrolID = 23 or PatrolID = 35 or PatrolID = 36 or PatrolID = 25 
			SET_MON_TASK_STAT 30004 1 1
			DESTROY_MON 1
		endif

		if PatrolID = 14
			MON_CHG_MAP 2000 31300 21200 16 20 1
			
		endif
		if PatrolID = 16 
			rand NumRand_2 4
			add NumRand_2 100
			switch NumRand_2
				case 100
					MON_CHG_MAP 2400 3200 3700 23 20 1
				endcase
				case 101
					MON_CHG_MAP 2400 3200 3700 35 20 1
				endcase
				case 102
					MON_CHG_MAP 2400 3200 3700 24 20 1
				endcase
				case 103
					MON_CHG_MAP 2400 3200 3700 36 20 1
				endcase
				
			endswitch
		endif
		
		if PatrolID = 24
			MON_CHG_MAP 3400 2855 3153 25 20 1
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
	GET_RAN_POS @MonX @MonY 700 OutRet OutMonX OutMonY
	;GET_RAN_POS #输入x坐标 #输入y坐标 #半径 #返回标志 #输出x坐标 #输出y坐标
;	if 11 = 0 
		MON_TASK_AREA_CHECK 30004 3000 300 200		
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
				strvar Rad
				rand Rad 2
				if Rad = 0
					TARGET_ADD_STATUS 3 @30 1009 2 20 1
					effect 0 0 playEffct($@OutMonX$,$@OutMonY$,"effect\\entity\\261004_18.ent")
					MON_CALL_ENEMY_MON 200026 2 45 2 @OutMonX @OutMonY 500 0 0 0 60 1 0
					MON_CALL_ENEMY_MON 200027 2 45 2 @OutMonX @OutMonY 500 0 0 0 60 1 0
				endif
				var 13 0
				SET_MON_VAR 1 8 @13 1		
			endif
		endif

	endif

endproc
