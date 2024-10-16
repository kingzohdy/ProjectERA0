;公会守护枢纽活动    ,副本
proc 1
	PWORLD_PUSH_WAVE_MON 1 1
	PWORLD_PUSH_WAVE_MON 1 2
	PWORLD_PUSH_WAVE_MON 1 3
	PWORLD_PUSH_WAVE_MON 1 4
	PWORLD_PUSH_WAVE_MON 1 5
	PWORLD_PUSH_WAVE_MON 1 6
	PWORLD_PUSH_WAVE_MON 1 7
	PWORLD_PUSH_WAVE_MON 1 8
	PWORLD_PUSH_WAVE_MON 1 9
	PWORLD_PUSH_WAVE_MON 1 10
	PWORLD_PUSH_WAVE_MON 1 11
	PWORLD_PUSH_WAVE_MON 1 12
	PWORLD_PUSH_WAVE_MON 1 13
	PWORLD_PUSH_WAVE_MON 1 14
	PWORLD_PUSH_WAVE_MON 1 15
	PWORLD_PUSH_WAVE_MON 1 16
	PWORLD_PUSH_WAVE_MON 1 17
	PWORLD_PUSH_WAVE_MON 1 18
	PWORLD_PUSH_WAVE_MON 1 19
	PWORLD_PUSH_WAVE_MON 1 20
	PWORLD_PUSH_WAVE_MON 1 21
	PWORLD_PUSH_WAVE_MON 1 22
	PWORLD_PUSH_WAVE_MON 1 23
	PWORLD_PUSH_WAVE_MON 1 24
	PWORLD_PUSH_WAVE_MON 1 25
	PWORLD_PUSH_WAVE_MON 1 26
	PWORLD_PUSH_WAVE_MON 1 27
	PWORLD_PUSH_WAVE_MON 1 28
	PWORLD_PUSH_WAVE_MON 1 29
	PWORLD_PUSH_WAVE_MON 1 30
	PWORLD_PUSH_TIMER_MON 1 31
	PWORLD_PUSH_TIMER_MON 1 32
	PWORLD_PUSH_TIMER_MON 1 33
	PWORLD_PUSH_TIMER_MON 1 34
	PWORLD_PUSH_TIMER_MON 1 35
	PWORLD_PUSH_TIMER_MON 1 36
;	PWORLD_PUSH_TIMER_MON 1 37
;	PWORLD_PUSH_TIMER_MON 1 38
;	PWORLD_PUSH_TIMER_MON 1 39
;	PWORLD_PUSH_TIMER_MON 1 40
	;monmemid10,枢纽怪memid
	var 10 0
	set_pworld_var 0 32 @10 1
	;monstate11,枢纽怪状态 0未刷 1存活 2死亡
	var 11 0
	set_pworld_var 4 32 @11 1
	;succ12,是否已播放成功特效
	var 12 0
	set_pworld_var 8 32 @12 1
	;cool13,无敌冷却时间
	var 13 0
	set_pworld_var 12 32 @13 1
endproc
;tick
proc 3
;	is_pworld_over 1
;	if 1 = 1
;		return
;	endif
	strvar num
	get_mon_num 1 num
	if num >= 800
		add_sys_msg_start
		add_sys_msg 敌方军队已经完全占领该处，公会守护挑战失败，可通过传送门离开 
		set_sys_msg_mode 14 0 0
		map_msg_info
		call 101
		return
	endif
	
	;monstate11,枢纽怪状态 0未刷 1存活 2死亡
	var 11 0
	get_pworld_var 4 32 1 11
	if 11 = 2 
		call 101
		return
	endif
	is_pworld_over 1
	if 1 = 1
		return
	endif
	strvar wavedef
	strvar timerdef
	strvar wavenum
	strvar timernum
	GET_PWORLD_WAVE_MON_ROUND 1 wavedef
	GET_PWORLD_TIMER_MON_ROUND 1 timerdef
;	add_sys_msg_start
;	add_sys_msg $@wavedef$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	;succ12,是否已播放成功特效
	var 12 0
	get_pworld_var 8 32 1 12
	if wavedef = 30  and 12 = 0
		GET_PWORLD_WAVE_MON_ALIVE wavenum
		GET_PWORLD_timer_MON_ALIVE timernum
;		if wavenum = 0 and timernum = 0
;		add_sys_msg_start
;		add_sys_msg $@wavenum$
;		set_sys_msg_mode 3 0 0
;		map_msg_info
		if wavenum = 0
			effect 0 0 playUiEffect(0,0,0,"effect\\entity\\shengli_16.ent")
			call 100
			;succ12,是否已播放成功特效
			var 12 1
			set_pworld_var 8 32 @12 1
			SET_PWORLD_timer_MON_COUNT_DOWN 1 -1
			pworld_over 
		endif
	endif
endproc
proc 100
;	NEW_NPC 1 333048 12800 10300 0 pw105out.mac
;	add_sys_msg_start
;	add_sys_msg “守护公会枢纽”已结束，下次请按时参加。 
;	set_sys_msg_mode 4 0 0
;	map_msg_info
endproc
proc 101
	effect 0 0 playUiEffect(0,0,0,"effect\\entity\\shibai_16.ent")
	call 100
	pworld_over
endproc
proc 80
	;monmemid10,枢纽怪memid
	var 10 0
	get_pworld_var 0 32 1 10
	if 10 > 0
		mon_set_hp 100000 @10 1 
	endif
endproc
proc 81
	;monmemid10,枢纽怪memid
	var 10 0
	get_pworld_var 0 32 1 10
	add_sys_msg_start
	add_sys_msg 枢纽怪memid$@10$ 
	set_sys_msg_mode 3 0 0
	map_msg_info
endproc
