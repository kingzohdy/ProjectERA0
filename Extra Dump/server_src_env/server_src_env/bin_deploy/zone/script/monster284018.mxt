;邪念

proc 1 
	strvar Hp m_Hp mID
	GET_MON_INFO 0 0 mID Hp 0 0 0 0
	var m_Hp 0
	dev Hp 2
	add Hp 1000
	sub m_Hp @Hp
	MON_SET_HP @m_Hp @mID 0
endproc
proc 2

	strvar mID Ret_Hp
	GET_MON_INFO 0 0 mID 0 0 0 0 0
	GET_MON_HP_PERCENT @mID Ret_Hp
			
	if Ret_Hp != 100
	
		strvar MapLoc
		get_pworld_var 22 8 1 MapLoc
		var MapLoc 1
		set_pworld_var 22 8 @MapLoc 1
		
	endif
	
endproc 

proc 4
	strvar mID
	GET_MON_INFO 0 0 mID 0 0 0 0 0
	MON_SET_HP 100 @mID 0
	
	get_pworld_var 1 32 1 51
	add 51 1
	set_pworld_var 1 32 @51 1
		if 51 = 6
			strvar Mid
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			mon_speech @Mid 2 2 1 我的生命会随着时间慢慢恢复，保护我，直到我生命回满为止！
			add_sys_msg_start
			add_sys_msg 保护信念，不要让邪念接近，直至信念的生命回满
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		if 51 = 7
		
			MON_CALL_ENEMY_MON 284019 1 0 0 12514 14289 200 0 0 0 0 0 0	
			MON_CALL_ENEMY_MON 284019 1 0 0 11950 15189 200 0 0 0 0 0 0	
			MON_CALL_ENEMY_MON 284019 1 0 0 10406 14298 200 0 0 0 0 0 0	
			MON_CALL_ENEMY_MON 284019 1 0 0 10969 13304 200 0 0 0 0 0 0	
			
		endif
		if 51 = 17
			var 51 6
			set_pworld_var 1 32 @51 1
		endif
		
;			MON_CALL_ENEMY_MON 284019 1 0 0 12664 16470 200 0 0 0 0 0 0				
;			MON_CALL_ENEMY_MON 284019 1 0 0 13830 14293 200 0 0 0 0 0 0	
;;;;;;;;;;;;;;;;信念满血后
		get_pworld_var 6 8 1 Com
		if Com = 0
			strvar mID Ret_Hp
			GET_MON_INFO 0 0 mID 0 0 0 0 0
			GET_MON_HP_PERCENT @mID Ret_Hp
			
			if Ret_Hp = 100

				DELETE_MON_BY_DEFID 284019 1
				strvar Com
				get_pworld_var 6 8 1 Com
				var Com 1
				set_pworld_var 6 8 @Com 1
				
				DESTROY_MON 1
			else
			
				get_mon_var 0 8 1 50
				add 50 1
				set_mon_var 0 8 @50 1
				if 50 = 4
					add_sys_msg_start
					add_sys_msg 用挑衅技能强迫怪物攻击你，给信念争取足够的时间回满血！
					set_sys_msg_mode 3 0 0
					map_msg_info
					
					var 50 0
					set_mon_var 0 8 @50 1
				endif
			
			endif
		endif
endproc 



















