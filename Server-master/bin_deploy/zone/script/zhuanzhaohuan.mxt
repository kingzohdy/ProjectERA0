;转职竞技场召唤裁判



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b懦弱的人只会裹足不前，莽撞的人只能引火烧身，只有真正勇敢的人才能所向披靡。
			
	
	task_add_option 3406 1 100
	task_add_option 3407 1 101
	task_add_option 3408 1 102			
			
	strvar Ret Ret1 Ret2 
	task_stat 3406 1 Ret
	get_task_var 3406 0 8 1 10
	if Ret = 2 and 10 = 0
		option 140 #挑战埃里斯
	endif
	task_stat 3407 1 Ret
	get_task_var 3407 0 8 1 11
	if Ret = 2 and 11 = 0
		option 141 #挑战泽普世
	endif
	task_stat 3408 1 Ret
	get_task_var 3408 0 8 1 12
	if Ret = 2 and 12 = 0
		option 142 #挑战伊索
	endif
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	strvar Ret3169
	task_stat 3169 1 Ret3169
	if Ret3169 = 2
		option 200 #请助我召唤封印于竞技场中的影子
	endif
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	
	option 145 #回到管理员旁边
	talk
	
endproc 

PROC 100
	task_do_option 3406 1
ENDPROC
PROC 101
	task_do_option 3407 1
ENDPROC
PROC 102
	task_do_option 3408 1
ENDPROC


proc 140
		
	strvar Ret1 Ret2 Ret3 Ret4 Loc1
	GET_MAP_MON_NUM 4402 282023 1 Ret1 1
	GET_MAP_MON_NUM 4402 282024 1 Ret2 1
	GET_MAP_MON_NUM 4402 282025 1 Ret3 1
	GET_MAP_MON_NUM 4402 282027 1 Ret4 1
	get_map_val 0 8 1 Loc1
		
	if Ret1 < 1 and Ret2 < 1 and Ret3 < 1 and Ret4 < 1 and Loc1 = 0
;		NEW_MON 282023 1 13000 13400 0 0 0 0 0
		strvar Loc
		get_task_var 3406 0 8 1 Loc
		var Loc 1
		set_task_var 3406 0 8 @Loc 1	

		var Loc1 1
		set_map_val 0 8 @Loc1 1
		
	else
		add_sys_msg_start
		add_sys_msg 消灭场景中剩余的怪物再来挑战吧！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 141
	strvar Ret1 Ret2 Ret3 Ret4 Loc2
	GET_MAP_MON_NUM 4402 282023 1 Ret1 1
	GET_MAP_MON_NUM 4402 282024 1 Ret2 1
	GET_MAP_MON_NUM 4402 282025 1 Ret3 1
	GET_MAP_MON_NUM 4402 282027 1 Ret4 1
	get_map_val 1 8 1 Loc2
		
	if Ret1 < 1 and Ret2 < 1 and Ret3 < 1 and Ret4 < 1 and Loc2 = 0
;		NEW_MON 282023 1 13000 13400 0 0 0 0 0
		strvar Loc
		get_task_var 3407 0 8 1 Loc
		var Loc 1
		set_task_var 3407 0 8 @Loc 1	
		
		var Loc2 1
		set_map_val 1 8 @Loc2 1
	else
		add_sys_msg_start
		add_sys_msg 消灭场景中剩余的怪物再来挑战吧！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 142
	strvar Ret1 Ret2 Ret3 Ret4 Loc3
	GET_MAP_MON_NUM 4402 282023 1 Ret1 1
	GET_MAP_MON_NUM 4402 282024 1 Ret2 1
	GET_MAP_MON_NUM 4402 282025 1 Ret3 1
	GET_MAP_MON_NUM 4402 282027 1 Ret4 1
	get_map_val 2 8 1 Loc3
	if Ret1 < 1 and Ret2 < 1 and Ret3 < 1 and Ret4 < 1 and Loc3 = 0
;		NEW_MON 282023 1 13000 13400 0 0 0 0 0
		strvar Loc
		get_task_var 3408 0 8 1 Loc
		var Loc 1
		set_task_var 3408 0 8 @Loc 1		
		
		var Loc3 1
		set_map_val 2 8 @Loc3 1
	else
		add_sys_msg_start
		add_sys_msg 消灭场景中剩余的怪物再来挑战吧！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 145

	transmit 4402 10000 13300 1
endproc

proc 200
	strvar Num Ret Loc LocMap
	GET_MAP_MON_NUM 4402 205052 Ret Num 1
	get_map_val 10 8 1 LocMap
	
	if Ret = 0 and Num = 0 and LocMap != 10

		mon_speech 0 0 0 1 降临吧！黑暗中的精灵！汝之剑将开启封印之门！
		
		get_map_val 10 8 1 LocMap
		var LocMap 10
		set_map_val 10 8 @LocMap 1
		
		get_task_var 3169 0 8 1 Loc	
		var Loc 1
		set_task_var 3169 0 8 @Loc 1
	else
		add_sys_msg_start
		add_sys_msg 消灭竞技场中剩余的怪物再来挑战吧！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif

endproc 



proc 2
	get_trig_task 1 2
	if 2 = 3406
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b懦弱的人只会裹足不前，莽撞的人只能引火烧身，只有真正勇敢的人才能所向披靡。
		
			option 140 #挑战埃里斯
		talk
	
	endif  

	if 2 = 3407
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b懦弱的人只会裹足不前，莽撞的人只能引火烧身，只有真正勇敢的人才能所向披靡。
			option 141 #挑战泽普世
		talk
	endif 
	
	if 2 = 3408
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b懦弱的人只会裹足不前，莽撞的人只能引火烧身，只有真正勇敢的人才能所向披靡。
			option 142 #挑战伊索
		talk
	endif 
	
endproc 

 