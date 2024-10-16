;战士试炼师奥利夫

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当你明白一个战士肩负的责任，这场试炼就难不到你了。
	
	task_stat 3143 1 90
	get_task_var 3143 0 8 1 20

	
	if 20 = 0 and 90 = 2
		option 150 #试炼一：护送奥利夫
	endif
		
	get_task_var 3143 0 8 1 20
	if 20 = 1 and 90 = 2
		option 151 #试炼二：挑战夜幕之牙
	endif
	
		option 152 #回到时间城
	
	talk

endproc

proc 150
	DEL_NPC 302055 0
	CALL_MON_BY_PATROL 1 302059 0 0 13800 42900 70 0 10
;	strvar LV
;	GET_PLAYER_BASE_ATT 1 1 LV 1
;	MON_CALL_ENEMY_MON 302059 1 @LV 4 13800 42900 0 70 0 0 0 0 0
	get_task_var 3143 0 8 1 20
	var 20 1
	set_task_var 3143 0 8 @20 1
	
	ADD_SYS_MSG_START
	ADD_SYS_MSG 保护奥利夫到达悬空台，危急时不妨使用 “挑衅”技能。
	SET_SYS_MSG_MODE 1 0 0
	MSG_INFO
endproc
proc 151
	
;	DEL_NPC 302055 0
;	CALL_MON_BY_PATROL 1 272001 0 0 26200 46700 70 0 10
	get_task_var 3143 0 8 1 20
	var 20 2
	set_task_var 3143 0 8 @20 1
	
	GET_MAP_NPC_ID 302055 50 0 
	mon_speech @50 0 2 1 夜幕之牙强悍而又狡诈，你必须拿出所有的实力来战胜它。
	effect 0 0 playEffct(26200,46700,"effect\\entity\\9600_16.ent")
	
	ADD_SYS_MSG_START
	ADD_SYS_MSG 击败夜幕之牙，不妨使用 “反击风暴”技能自动进行反击，增加输出的伤害。
	SET_SYS_MSG_MODE 1 0 0
	MSG_INFO
	
endproc
proc 152
	transmit 1000 23800 27400 1
;	RESET_PWORLD 505
	task_stat 3143 1 30
	if 30 != 1
		TASK_FAIL 3143 1
	endif
endproc







