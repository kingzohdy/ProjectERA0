proc 1
	strvar Loc1
	var Loc1 0
	set_task_var 3162 8 8 @Loc1 1
	;进入副本后开始检查地图

endproc 

proc 2
		GET_MAP_NPC_ID 302057 50 0 
		mon_speech @50 0 2 1 真意外，竟然还有人能从我手中通过，为你的侥幸庆幸吧。
endproc
;枪手试炼二
proc 4
;失败

	DELETE_MON_BY_DEFID 263001 1
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50600	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 506
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
;		pworld_over
		DELETE_MON_BY_DEFID 262001 1
	endif
endproc 
proc 5
;放弃
	DELETE_MON_BY_DEFID 263001 1
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50600	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 506
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
;		pworld_over
		DELETE_MON_BY_DEFID 262001 1
	endif
endproc

proc 6

	get_task_var 3162 8 8 1 Loc1
	if Loc1 = 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 1 1 1 
		if Map != 50600
			TASK_FAIL 3162 1
			return
		endif
	endif
	;传出地图任务失败


	get_task_var 3162 0 8 1 20
	if 20 = 1
		get_task_var 3162 1 8 1 21
		add 21 1
		set_task_var 3162 1 8 @21 1
		if 21 = 1
			
			effect 0 0 playEffct(37500,19400,"effect\\entity\\9591_16.ent")
			effect 0 0 playEffct(37500,20200,"effect\\entity\\9591_16.ent")
			effect 0 0 playEffct(37500,18700,"effect\\entity\\9591_16.ent")
		endif
		if 21 = 3
			NEW_MON 262001 5 37500 19400 500 0 0 0 0
			NEW_MON 262001 5 37500 20200 500 0 0 0 0
			NEW_MON 262001 5 37500 18700 500 0 0 0 0
		endif
		if 21 <= 13 and 21 > 3
			
			sub 21 3	
			add_sys_msg_start
			add_sys_msg 计时：$@21$/10秒！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 21 = 14	
			strvar Ret
			TASK_FAIL 3162 Ret
			if Ret = -1
				return
			else
				add_sys_msg_start
				add_sys_msg 很可惜，你没能清理10只怪物！
				set_sys_msg_mode 1 0 0
				msg_info
			endif
			
			get_task_var 3162 0 8 1 20
			var 20 2
			set_task_var 3162 0 8 @20 1
			
		endif
	endif
endproc 
