;黑雾

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
		if 51 = 5
			strvar Mid
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			mon_speech @Mid 2 2 1 我的生命会随着时间慢慢恢复，保护我，直到我生命回满为止！
			add_sys_msg_start
			add_sys_msg 保护信念，直到信念的生命回满
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		if 51 = 6
			strvar Rad1 Rad2 Rad3
			var Rad1 0
			var Rad2 2
			while @Rad1 < @Rad2
			   add Rad1 1
			   
			   rand Rad3 4
				if Rad3 = 0
					MON_CALL_ENEMY_MON 284021 1 0 0 19194 7580 0 0 0 0 0 0 0	
				endif
				if Rad3 = 1
				
					MON_CALL_ENEMY_MON 284021 1 0 0 19178 9227 0 0 0 0 0 0 0	
				endif
				if Rad3 = 2
				
					MON_CALL_ENEMY_MON 284021 1 0 0 20779 9233 0 0 0 0 0 0 0	
				endif
				if Rad3 = 3
				
					MON_CALL_ENEMY_MON 284021 1 0 0 20849 7420 0 0 0 0 0 0 0	
				endif
			endwhile
		endif
		if 51 = 11
			var 51 5
			set_pworld_var 1 32 @51 1
		endif
		
		;;;;;信念满血后
		
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
					add_sys_msg 击杀所有出现的怪物，给信念争取足够的时间回满血！别让怪物靠近你哟！
					set_sys_msg_mode 3 0 0
					map_msg_info
					
					var 50 0
					set_mon_var 0 8 @50 1
				endif
			
			endif
		endif
endproc 



















