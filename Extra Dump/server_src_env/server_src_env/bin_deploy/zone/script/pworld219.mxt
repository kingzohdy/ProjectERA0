;副本 - 盗梦空间
;创建副本事件
proc 1
	PWORLD_PUSH_WAVE_MON 1 101
	PWORLD_PUSH_WAVE_MON 1 102
	PWORLD_PUSH_WAVE_MON 1 103
	PWORLD_PUSH_WAVE_MON 1 104
	PWORLD_PUSH_WAVE_MON 1 105
	PWORLD_PUSH_WAVE_MON 1 106
	PWORLD_PUSH_WAVE_MON 1 107
	PWORLD_PUSH_WAVE_MON 1 108
	PWORLD_PUSH_WAVE_MON 1 109
	PWORLD_PUSH_WAVE_MON 1 110
	PWORLD_PUSH_WAVE_MON 1 111
	PWORLD_PUSH_WAVE_MON 1 112
	PWORLD_PUSH_WAVE_MON 1 113
	PWORLD_PUSH_WAVE_MON 1 114
	PWORLD_PUSH_WAVE_MON 1 115
	PWORLD_PUSH_WAVE_MON 1 116
	PWORLD_PUSH_WAVE_MON 1 117
	PWORLD_PUSH_WAVE_MON 1 118
	
	;monmemid10,保存要保护的NPC[雅典娜]的实例ID
	var 10 0
	set_pworld_var 0 32 @10 1
	
	;monstate11,要保护的NPC状态 0未刷 1存活 2死亡
	var 11 0
	set_pworld_var 4 32 @11 1
	
	;新加计时器
	var 12 0
	set_pworld_var 8 8 @12 1
	var 13 0
	set_pworld_var 12 32 @13 1
	
	
	
endproc

;关闭副本事件
proc 2

endproc

;副本定时事件（每2秒触发一次）
proc 3
	;判断副本是否结束
	is_pworld_over 1
	if 1 = 1
		return
	endif
	
	;判断要保护的NPC是否死亡
	get_pworld_var 4 32 1 10
	
	if 10 = 2
		call 100
		return
	endif
	
	strvar wavedef
	strvar wavenum
	GET_PWORLD_WAVE_MON_ROUND 1 wavedef

	if wavedef = 118
		GET_PWORLD_WAVE_MON_ALIVE wavenum
		if wavenum = 0
			;effect 0 0 playUiEffect(0,0,0,"effect\\entity\\shengli_16.ent")
			call 110
			OPEN_PWORLD_AWARD_UI
		endif
	endif
	
	
	get_pworld_var 8 8 1 12 
	
	if 12 = 1
		;boss 死亡
		
		get_pworld_var 12 32 1 13
		add 13 1
		set_pworld_var 12 32 @13 1
		
		if 13 = 1
		
			add_sys_msg_start
			add_sys_msg 筑梦空间已通关，120秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 13 = 30
			add_sys_msg_start
			add_sys_msg 筑梦空间已通关，60秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 13 = 45
			add_sys_msg_start
			add_sys_msg 筑梦空间已通关，30秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 13 = 55
			add_sys_msg_start
			add_sys_msg 筑梦空间已通关，10秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 13 = 60
			pworld_over
			COMPEL_CLOSE_PWORLD 1
			return
		endif
	
	endif
endproc

;副本中怪物死亡事件
proc 4
endproc

;副本中角色进入事件
proc 5

endproc

;副本中角色死亡事件
proc 6

endproc

;副本中倒计时结束事件
proc 7

endproc

proc 100
	effect 0 0 playUiEffect(0,0,0,"effect\\entity\\shibai_16.ent")
	call 110
	pworld_over
endproc

proc 110
;	pworld_over
	SET_PWORLD_TIMER_W_COUNT_DOWN 1 -1
endproc