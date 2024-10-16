;迷之塔
;第五层

;创建副本事件
PROC 1
	;时间 每20s处理
	set_pworld_var 0 32 0 0
	;第几波
	set_pworld_var 4 32 0 0 
	;0表示不能刷波次怪，1表示可以刷
	set_pworld_var 8 32 0 0
	;延迟1分钟退出副本
	set_pworld_var 12 32 0 0
	;1表示开始进行延迟标记
	set_pworld_var 16 32 0 0
	;每一分钟处理
	set_pworld_var 20 32 0 0
ENDPROC

;关闭副本事件
proc 2
endproc

;副本定时事件（每2秒触发一次）
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		get_pworld_var 4 32 1 12
		get_pworld_var 8 32 1 60
		
		if @12 = 0 and @60 = 0
			set_pworld_var 8 32 1 1 
		elseif @12 = 1 and @60 = 0
			set_pworld_var 8 32 1 1 
			set_pworld_var 0 32 0 0
		elseif @12 = 2 and @60 = 0
			get_mon_num_by_defid 212120 20
			;看第一波死光没
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 3 and @60 = 0
			get_mon_num_by_defid 212121 20
			;看第二波死光没
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 4 and @60 = 0
			get_mon_num_by_defid 212122 20
			;看第三波死光没
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 5 and @60 = 0
			get_mon_num_by_defid 212123 20
			;看第四波死光没
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 6 and @60 = 0
			get_mon_num_by_defid 212124 20
			;看第五波死光没
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 7 and @60 = 0
			get_mon_num_by_defid 212005 20
			;看boss死了没
			if @20 < 1
				set_pworld_var 16 32 1 1
				set_pworld_var 8 32 1 1 
			endif
		else
		endif
		
		;开始延迟
		get_pworld_var 16 32 1 91
		if @91 = 1
			get_pworld_var 12 32 1 90
			;1分钟后全部退出副本
			if @90 >= 30
				pworld_over
				COMPEL_CLOSE_PWORLD 1
			else
				add 90 1
				set_pworld_var 12 32 @90 1
				
				if @90 = 1
					new_npc 1 333160 12889 12550 0 csm333160.mac			
					add_sys_msg_start
					add_sys_msg 副本已完成，可通过传送门离开
					set_sys_msg_mode 3 0 0
					map_msg_info
				endif
			endif
		else
			get_pworld_var 4 32 1 91
			if @91 >= 7
				return
			endif
			
			;刷怪
			get_pworld_var 0 32 1 11
			if @11 >= 10
				get_pworld_var 8 32 1 50
				if @50 = 1
					call 100
				endif
				set_pworld_var 0 32 0 0
			else
				add 11 1
				set_pworld_var 0 32 @11 1
			endif
			
			;每1分钟随机一个玩家获得一个buff
			get_pworld_var 20 32 1 18
			if @18 >= 27
				call 101
				set_pworld_var 20 32 0 0
			else
				add 18 1
				set_pworld_var 20 32 @18 1
			endif
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

;副本活动开始事件
proc 8
endproc 

proc 100
	strvar flag
	var flag 0
	
	;等这波怪死亡后才能刷下一波
	set_pworld_var 8 32 0 0
	
	get_pworld_var 4 32 1 13
	if @13 = 0
		add_sys_msg_start
		add_sys_msg 20秒后将刷出第一波怪物，勇士们，准备好战斗吧！
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	else
		if @13 < 6
			add_sys_msg_start
			add_sys_msg 随机玩家将中对怪物增益的效果，获得效果的玩家远离怪物人群
			set_sys_msg_mode 4 0 0
			MAP_MSG_INFO
		endif
		
		if @13 = 1
			call 201
		elseif @13 = 2
			call 202
		elseif @13 = 3
			call 203
		elseif @13 = 4
			call 204
		elseif @13 = 5
			call 205
		elseif @13 = 6
			;boss
			call 206
		else
			var flag 1 
		endif
	endif
	
	if @flag = 0
		add 13 1
		set_pworld_var 4 32 @13 1
	endif
endproc

proc 101
	strvar iMemID
	GET_RAND_PWORLD_PLAYER_ID 10 iMemID
	if @10 = 0
		TARGET_ADD_STATUS 1 @iMemID 2111 1 10 0
	endif
endproc

proc 201
	NEW_LIFE_MON 212120 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212120 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212120 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212120 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212120 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 202
	NEW_LIFE_MON 212121 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212121 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212121 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212121 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212121 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 203
	NEW_LIFE_MON 212122 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212122 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212122 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212122 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212122 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 204
	NEW_LIFE_MON 212123 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212123 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212123 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212123 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212123 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 205
	NEW_LIFE_MON 212124 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212124 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212124 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212124 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212124 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 206
	NEW_LIFE_MON 212005 1 12889 12550 0 0 0 0 0 0 0
endproc
