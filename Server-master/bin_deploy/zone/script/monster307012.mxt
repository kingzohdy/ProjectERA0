;;;飞碟1号



proc 1
	strvar mid
	GET_MON_INFO 1 1 mid 1 1 1 1 1
	TARGET_ADD_STATUS 3 @mid 1009 2 10 1
	MON_SPEECH @mid 1 1 1 开始探测宝藏！
	
	strvar Loc val_2 val_3 val_4 Time
	var Loc 0
	set_mon_var 0 8 @Loc 1
	var Time 0
	set_mon_var 1 8 @Time 1
	
endproc 

proc 2
	;得到公会ID
	strvar ClanID mhp mx my
	get_pworld_var 0 32 1 ClanID	
	GET_MON_INFO 1 1 1 mhp 1 1 mx my

	add_sys_msg_start
	if mhp != 0	
		add_sys_msg 宝藏探测器能源耗尽，寻宝活动结束！ 
	else
		add_sys_msg 宝藏探测器受损严重，无法正常搜寻，寻宝活动结束！
	endif
	set_sys_msg_mode 14 0 0
	CLAN_MSG_INFO 1 @ClanID
	
	;活动置为已结束
	DYN_ACTIVE_STATUS_SET 1 19 2
	
	;不可挖宝
	strvar Locw
	get_pworld_var 100 8 1 Locw
	var Locw 0
	set_pworld_var 100 8 @Locw 1
	
	
	;重置副本变量
	
	set_pworld_var 64 8 0 1
	set_pworld_var 100 8 0 1 	
	set_pworld_var 101 8 0 1
	
	set_pworld_var 4 32 0 1 
	set_pworld_var 8 32 0 1  
	set_pworld_var 12 32 0 1
	set_pworld_var 16 32 0 1
	
endproc
proc 3
;巡逻点进入
endproc 

proc 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;挖出怪物主动攻击飞碟
	
	strvar LocMon px py
	get_pworld_var 101 8 1 LocMon
	if LocMon = 10
		get_pworld_var 12 32 1 px 
		get_pworld_var 16 32 1 py
		
		;NEW_LIFE_MON 284272 1 @px @py 0 0 0 0 60 0 0
		
		
;		MON_CALL_ENEMY_MON 284272 1 60 2 @px @py 0 0 0 0 60 1 0
	;	effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\1000900_8.ent")

		var LocMon 0
		set_pworld_var 101 8 @LocMon 1
		
	endif
		
;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar mid midx midy
	GET_MON_INFO 1 1 mid 1 1 1 1 1
	
;;;;;;;;;;;;;;;;;;;;;;;锁，飞碟静止1分钟的时候跳出timer
	get_mon_var 0 8 1 Loc 
	if Loc = 1
		get_mon_var 1 8 1 Time
		add Time 1
		set_mon_var 1 8 @Time 1
		if Time < 20
			if Time = 7
				MON_SPEECH @mid 1 1 1 附近发现宝藏！附近发现宝藏！
			endif
			if Time = 14
				MON_SPEECH @mid 1 1 1 附近发现宝藏！附近发现宝藏！
			endif
			return
		else
			get_mon_var 0 8 1 Loc 	
			var Loc 0
			set_mon_var 0 8 @Loc 1
			
			get_mon_var 1 8 1 Time
			var Time 0
			set_mon_var 1 8 @Time 1
			
			;不可挖宝标记
			strvar Locw
			get_pworld_var 100 8 1 Locw
			var Locw 0
			set_pworld_var 100 8 @Locw 1
			
		endif
	endif
	
;;;;;;;;;;;;;;;;;;;;;;计时 90秒到180秒之间随即停留
	get_mon_var 1 8 1 Time
	add Time 1
	set_mon_var 1 8 @Time 1
	if Time = 1
		
		get_mon_var 1 8 1 Time
		rand Time 30
		set_mon_var 1 8 @Time 1
	endif
	
	;add_sys_msg_start
	;add_sys_msg $@Time$
	;set_sys_msg_mode 1 0 0
	;map_msg_info
	
	if Time = 40

		TARGET_ADD_STATUS 3 @mid 1009 2 60 1
		
		
		strvar ClanID
		get_pworld_var 0 32 1 ClanID	
		
		add_sys_msg_start
		add_sys_msg 探测器发现地下蕴藏的宝藏，大家可以在探测器附近用挖掘器进行挖掘！
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @ClanID
		
		MON_SPEECH @mid 1 1 1 附近发现宝藏！附近发现宝藏！
		;记录飞碟发现宝藏时的坐标,用于判断人物与飞碟的距离
		
		strvar midx midy
		GET_MON_INFO 1 1 1 1 1 1 midx midy
		set_pworld_var 4 32 @midx 1
		set_pworld_var 8 32 @midy 1
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		;可以挖宝标记
		strvar Locw
		get_pworld_var 100 8 1 Locw
		var Locw 1
		set_pworld_var 100 8 @Locw 1
		
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		get_mon_var 1 8 1 Time
		var Time 0
		set_mon_var 1 8 @Time 1
		
		get_mon_var 0 8 1 Loc 	
		var Loc 1
		set_mon_var 0 8 @Loc 1
	endif

		
endproc  











