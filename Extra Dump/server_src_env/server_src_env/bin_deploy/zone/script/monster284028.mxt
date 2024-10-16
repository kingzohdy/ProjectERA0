;镇压者 噬魂者

proc 1 
	strvar Hp m_Hp mID Mid
	GET_MON_INFO 0 0 mID Hp 0 0 0 0
	var m_Hp 0
	dev Hp 2
	add Hp 7000
	sub m_Hp @Hp
	MON_SET_HP @m_Hp @mID 0
	
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @Mid 1253 1 7200 1
	
	
	var 50 0
	set_mon_var 0 8 @50 1
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
;	MON_SET_HP 300 @mID 0
	
	get_pworld_var 1 32 1 51
	add 51 1
	set_pworld_var 1 32 @51 1

	if 51 = 2
			strvar Mid
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			mon_speech @Mid 2 2 1 善于利用自己的天赋，祭司，让我感受圣光的力量。
			add_sys_msg_start
			add_sys_msg 善于利用自己的天赋，祭司，让我感受圣光的力量。
			set_sys_msg_mode 3 0 0
			map_msg_info
	endif
	if 51 = 3
			MON_CALL_ENEMY_MON 284023 1 0 0 19194 7580 0 0 0 0 0 0 0	
			
			MON_CALL_ENEMY_MON 284022 1 0 0 19178 9227 0 0 0 0 0 0 0	
			
			MON_CALL_ENEMY_MON 284023 1 0 0 20779 9233 0 0 0 0 0 0 0	
			
			MON_CALL_ENEMY_MON 284022 1 0 0 20849 7420 0 0 0 0 0 0 0	
	endif
	if 51 = 13
		var 51 2
		set_pworld_var 1 32 @51 1
	endif
	
		
;;;;;;;;;;;;;;;;;;;;;信念满血后
		
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
					add_sys_msg 出怪时优先击杀怪物！然后圣光术治疗信念，使信念回满血！
					set_sys_msg_mode 3 0 0
					map_msg_info
					
					var 50 0
					set_mon_var 0 8 @50 1
				endif
			endif
		endif
endproc 



















