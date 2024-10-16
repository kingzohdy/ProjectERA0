;祭祀试炼
proc 1
	strvar Loc1
	var Loc1 0
	set_task_var 3148 8 8 @Loc1 1
	;进入副本后开始检查地图

endproc 
proc 2
	call 100
endproc 

proc 4
;任务失败
	call 100
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50700	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 507	
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac	
;		NEW_NPC 1 333030 37500 19400 0 zyzqgotozc.mac
		pworld_over
	endif
endproc

proc 5
;放弃任务
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50700	
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 507	
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
		pworld_over
		
		get_pworld_var 36 32 1 76
		var 76 1
		set_pworld_var 36 32 @76 1
		
		get_task_var 3148 0 8 1 20
		var 20 10
		set_task_var 3148 0 8 @20 1
		
		get_task_var 3148 2 8 1 22
		var 22 10
		set_task_var 3148 2 8 @22 1
		
	endif

	call 100
endproc
proc 100
	get_pworld_var 0 32 1 23
	get_pworld_var 4 32 1 60
	get_pworld_var 8 32 1 61
	var 23 0
	set_pworld_var 0 32 @23 1
	var 60 0
	set_pworld_var 4 32 @60 1
	var 61 0
	set_pworld_var 8 32 @61 1
	
	get_pworld_var 12 32 1 70
	get_pworld_var 16 32 1 71
	get_pworld_var 20 32 1 72
	get_pworld_var 24 32 1 73
	get_pworld_var 28 32 1 74
	get_pworld_var 32 32 1 75
	var 70 0
	set_pworld_var 12 32 @70 1
	var 71 0
	set_pworld_var 16 32 @71 1
	var 72 0
	set_pworld_var 20 32 @72 1
	var 73 0
	set_pworld_var 24 32 @73 1
	var 74 0
	set_pworld_var 28 32 @74 1
	var 75 0
	set_pworld_var 32 32 @75 1
	
endproc 
proc 6

	get_task_var 3148 8 8 1 Loc1
	if Loc1 = 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 1 1 1 
		if Map != 50700
			TASK_FAIL 3148 1
			return
		endif
	endif
	;传出地图任务失败
	;;;;;;;;;;;;;;;;;;;;;;;;;
	get_task_var 3148 0 8 1 20
	if 20 = 1
	
		get_task_var 3148 1 8 1 21
		add 21 1
		set_task_var 3148 1 8 @21 1
		if 21 = 4
			add_sys_msg_start
			add_sys_msg 计时开始！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 21 = 5
	;;;;;;;;;;一次性扣血
			get_pworld_var 12 32 1 70
			MON_SET_HP -600 @70 1		
			get_pworld_var 16 32 1 71
			MON_SET_HP -1200 @71 1	
			get_pworld_var 20 32 1 72
			MON_SET_HP -1800 @72 1	
			
			get_pworld_var 24 32 1 73
			MON_SET_HP -600 @73 1		
			get_pworld_var 28 32 1 74
			MON_SET_HP -1200 @74 1	
;			get_pworld_var 32 32 1 75
;			MON_SET_HP -1800 @75 1				
		endif
		if 21 <= 32 and 21 > 4
			var 25 @21
			sub 25 3
			
			add_sys_msg_start
			add_sys_msg 计时：$@25$/30秒！
			set_sys_msg_mode 3 0 0
			msg_info
			
			get_pworld_var 4 32 1 60
			var 60 1
			set_pworld_var 4 32 @60 1
		
		endif
		if 21 = 33
			add_sys_msg_start
			add_sys_msg 计时结束！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 21 = 34
			add_sys_msg_start
			add_sys_msg 厉害，居然救活了那么多山羊，继续进行下一阶段的试炼吧！
			set_sys_msg_mode 1 0 0
			msg_info
			
			GET_MAP_NPC_ID 302053 50 0 
			mon_speech @50 0 2 1 感谢你的努力，卷角山羊才得以最大程度的存活下来，但是我们的任务还没有结束，准备好后来找我。
			get_task_var 3148 2 8 1 22
			var 22 1
			set_task_var 3148 2 8 @22 1
			
			get_task_var 3148 0 8 1 20
			var 20 2
			set_task_var 3148 0 8 @20 1
		
			get_pworld_var 4 32 1 60
			var 60 0
			set_pworld_var 4 32 @60 1	
			
		endif
	endif
	
	
	get_task_var 3148 8 8 1 Loc1
	if Loc1 = 1
	
		get_pworld_var 0 32 1 23
		if 23 = 1
			task_fini 3148 1
			
			GET_MAP_NPC_ID 302053 50 0 
			mon_speech @50 0 2 1 感谢你的帮助，偷猎者都已经被驱逐了，试炼也到此结束，祝贺你成功通过，愿主保佑你。
		
			get_task_var 3148 2 8 1 22
			var 22 2
			set_task_var 3148 2 8 @22 1
		
		endif
		get_pworld_var 8 32 1 61

		if 61 >= 2
			TASK_FAIL 3148 1
			var 61 0
			set_pworld_var 8 32 @61 1
			
			get_pworld_var 4 32 1 60
			var 60 0
			set_pworld_var 4 32 @60 1
		endif
		
	endif
endproc 


