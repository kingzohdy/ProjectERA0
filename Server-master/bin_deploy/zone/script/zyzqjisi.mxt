;祭祀试炼师洛维奇

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我们不能只为自己而活，扶救弱势消除罪恶是我们的职责与义务。
		
		
		get_task_var 3148 0 8 1 20
		task_stat 3148 1 25
		if 20 = 0 and 25 = 2
			option 150 #试炼一：抢救独角山羊
		endif
		
		get_task_var 3148 2 8 1 22
		task_stat 3148 1 25
		if 22 = 1 and 25 = 2
			option 151 #试炼二：保护洛维奇
		endif
		
		option 152 #回到时间城
	talk

endproc
proc 150


		get_pworld_var 4 32 1 60
		var 60 0
		set_pworld_var 4 32 @60 1

		get_task_var 3148 0 8 1 20
		var 20 1
		set_task_var 3148 0 8 @20 1
			
		GET_MAP_NPC_ID 302053 50 0 
		mon_speech @50 0 2 1 请尽最大努力拯救这些可怜的卷角山羊，至少确保死亡数目在三只以下。
		
		add_sys_msg_start
		add_sys_msg 30秒内合理使用 “圣光术”与“圣女祷言”技能拯救卷角山羊，确保死亡数目小于2头。
		set_sys_msg_mode 1 0 0	
		msg_info 
		
		add_sys_msg_start
		add_sys_msg 3秒后开始救助卷角山羊
		set_sys_msg_mode 3 0 0	
		msg_info 
		
endproc 

proc 151
	DEL_NPC 302056 0
	CALL_MON_BY_PATROL 1 302060 0 0 13500 43700 71 0 10
	
	add_sys_msg_start
	add_sys_msg 保护洛维奇驱逐偷猎者，危急时不妨使用 “圣光盾”与“圣光术”保护她。
	set_sys_msg_mode 1 0 0	
	msg_info 
		
;	strvar LV
;	GET_PLAYER_BASE_ATT 1 1 LV 1
;	MON_CALL_ENEMY_MON 302060 1 @LV 4 13500 43700 0 71 0 0 0 0 0
endproc 
	
proc 152
	transmit 1000 23800 27400 1
;	RESET_PWORLD 507
	task_stat 3148 1 30
	if 30 != 1
		TASK_FAIL 3148 1
	endif
	
	
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





















