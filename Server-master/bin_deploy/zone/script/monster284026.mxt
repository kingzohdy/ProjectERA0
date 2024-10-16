;鬼火、火魂、冰魂

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
		;
		if 51 = 3
			strvar Mid
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			mon_speech @Mid 2 2 1 我的生命会随着时间慢慢恢复，保护我，直到我生命回满为止！
			
			add_sys_msg_start
			add_sys_msg 善于利用冰火相生相克之特性，保护信念不受伤害，直到信念生命回满
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		if 51 = 4
		
			strvar Rad1 Rad2 Rad3 Rad4 Mid
			rand Rad1 3
			if Rad1 = 0
				MON_CALL_ENEMY_MON 284025 1 0 0 19194 7580 200 0 0 0 0 Mid 0	

			endif
			if Rad1 = 1
				MON_CALL_ENEMY_MON 284024 1 0 0 19194 7580 200 0 0 0 0 Mid 0	
	
			endif
			if Rad1 = 2
				MON_CALL_ENEMY_MON 284020 1 0 0 19194 7580 200 0 0 0 0 0 0	
			endif
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			rand Rad2 3
			if Rad2 = 0
				MON_CALL_ENEMY_MON 284020 1 0 0 19178 9227 200 0 0 0 0 0 0	
			endif
			if Rad2 = 1
				MON_CALL_ENEMY_MON 284024 1 0 0 19178 9227 200 0 0 0 0 Mid 0
			endif
			if Rad2 = 2
				MON_CALL_ENEMY_MON 284025 1 0 0 19178 9227 200 0 0 0 0 Mid 0
			endif
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			
			rand Rad3 3
			if Rad3 = 0
				MON_CALL_ENEMY_MON 284020 1 0 0 20779 9233 200 0 0 0 0 0 0	
			endif
			if Rad3 = 1
				MON_CALL_ENEMY_MON 284024 1 0 0 20779 9233 200 0 0 0 0 Mid 0	
			endif
			if Rad3 = 2
				MON_CALL_ENEMY_MON 284025 1 0 0 20779 9233 200 0 0 0 0 Mid 0
			endif
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			rand Rad4 3
			if Rad4 = 0
				MON_CALL_ENEMY_MON 284020 1 0 0 20849 7420 200 0 0 0 0 0 0	
			endif
			if Rad4 = 1
				MON_CALL_ENEMY_MON 284024 1 0 0 20849 7420 200 0 0 0 0 Mid 0	
			endif
			if Rad4 = 2
				MON_CALL_ENEMY_MON 284025 1 0 0 20849 7420 200 0 0 0 0 Mid 0	
			endif
		
		endif
		if 51 = 14
			
			var 51 3
			set_pworld_var 1 32 @51 1
		endif
		
		;;;;;怪物满血后
		
		get_pworld_var 6 8 1 Com
		if Com = 0
			strvar mID Ret_Hp
			GET_MON_INFO 0 0 mID 0 0 0 0 0
			GET_MON_HP_PERCENT @mID Ret_Hp
			
			if Ret_Hp = 100

				DELETE_MON_BY_DEFID 282020 1
				DELETE_MON_BY_DEFID 282024 1
				DELETE_MON_BY_DEFID 282025 1
			
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
					add_sys_msg 用冰系法术击杀火魄、火系法术击杀冰魂，给信念争取足够的时间回满血！
					set_sys_msg_mode 3 0 0
					map_msg_info
					
					var 50 0
					set_mon_var 0 8 @50 1
				endif
			endif
		endif
endproc 



















