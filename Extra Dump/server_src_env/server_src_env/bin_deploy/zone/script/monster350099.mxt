;运镖
 
proc 1
;怪物刷出
	var 10 0
	SET_MON_VAR 0 32 @10 1
	STRVAR MonX
	STRVAR MonY
	STRVAR MonID
endproc


proc 2
;怪物死亡

			SET_MON_TASK_STAT 9997 2 1

endproc


proc 3
;怪物巡逻结束事件

	GET_MON_INFO 1 40 41 42 43 44 45 46
	if 43 = 2100 and 40 = 350099

			SET_MON_TASK_STAT 9997 1 1
			DEL_MON 1
	else

			MON_CHG_MAP 2100 4110 3912 18 20 1
	
	endif

endproc

proc 4
;3秒执行一次


	MON_TASK_AREA_CHECK 9997 2000 1 200
	;检查有效玩家
	GET_MON_VAR 0 32 1 10
	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
	
	;获得玩家等级
	
	add 10 1
	
	if 10 = 5 
		;15秒时让镖车停下30秒召唤怪物
		TARGET_ADD_STATUS 3 @30 1009 1 10 1
		
		MON_CALL_ENEMY_MON 200007 5 30 4 @MonX @MonY 1000 0 0 0 10 1 0
	endif
	set_mon_var 0 32 @10 1
	if 10 = 20 
		;50秒时让镖车停下30秒召唤怪物
		TARGET_ADD_STATUS 3 @30 1009 1 10 1
		MON_CALL_ENEMY_MON 200007 5 30 4 @MonX @MonY 1000 0 0 0 10 1 0
	
	endif
	set_mon_var 0 32 @10 1
	if 10 = 30 
		;让镖车停下30秒召唤怪物
		TARGET_ADD_STATUS 3 @30 1009 1 10 1
		MON_CALL_ENEMY_MON 200007 5 30 4 @MonX @MonY 1000 0 0 0 10 1 0
	
	endif
	set_mon_var 0 32 @10 1
	if 10 = 50 
		;让镖车停下30秒召唤怪物
		TARGET_ADD_STATUS 3 @30 1009 1 10 1
		MON_CALL_ENEMY_MON 200007 5 30 4 @MonX @MonY 1000 0 0 0 10 1 0
	
	endif
	set_mon_var 0 32 @10 1
endproc




















