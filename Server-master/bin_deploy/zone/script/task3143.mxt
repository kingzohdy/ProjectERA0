;战士试炼任务
proc 1
	strvar Loc1
	var Loc1 0
	set_task_var 3143 8 8 @Loc1 1
	;进入副本后开始检查地图


endproc 


proc 4
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50500	
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
			
;		NEW_NPC 1 333030 13400 44500 0 zyzqgotozc.mac
		pworld_over
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 505	
	endif
endproc 

proc 5	
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50500	
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 505	
		pworld_over
		
		get_pworld_var 0 32 1 30
		var 30 1
		set_pworld_var 0 32 @30 1
		
		get_task_var 3143 0 8 1 20
		var 20 10
		set_task_var 3143 0 8 @20 1
		
	endif
endproc 
proc 6
	
	get_task_var 3143 8 8 1 Loc1
	if Loc1 = 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 1 1 1 
		if Map != 50500
			TASK_FAIL 3143 1
			return
		endif
	endif
	;传出地图任务失败
		get_task_var 3143 0 8 1 20
		if 20 = 2
			get_task_var 3143 1 8 1 21
			add 21 1
			set_task_var 3143 1 8 @21 1
			if 21 = 3
;				effect 0 0 playEffct(26200,46700,"effect\\entity\\9590_16.ent")
				CALL_MON_BY_PATROL 1 272001 0 0 26200 46700 70 0 10
				
				var 20 3
				set_task_var 3143 0 8 @20 1
			endif
		
		endif

endproc 












