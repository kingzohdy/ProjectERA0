
;法师职业坐骑试炼任务
proc 1
	strvar Loc1
	var Loc1 0
	set_task_var 3158 8 8 @Loc1 1
	;进入副本后开始检查地图	
	
	var 20 0
	set_task_var 3158 0 8 @20 1
endproc


proc 100
	get_pworld_var 0 32 1 30
	get_pworld_var 4 8 1 31
	get_pworld_var 5 8 1 32
	get_pworld_var 7 8 1 34
	var 30 0
	set_pworld_var 0 32 @30 1
	var 31 0
	set_pworld_var 4 8 @31 1
	var 32 0
	set_pworld_var 5 8 @32 1
	var 34 0
	set_pworld_var 7 8 @34 1


endproc 

proc 4
	call 100
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50800
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 508
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
		pworld_over
	endif
	
	get_task_var 3158 0 8 1 20
	var 20 10
	set_task_var 3158 0 8 @20 1
	GET_MAP_NPC_ID 302058 50 0 
	EFFECT_ID 0 @50 stopMontion(%d,"294005")
	

	
endproc 
proc 5
	call 100
	strvar Map
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	if Map = 50800
;		transmit 1000 31200 29500 1
;		RESET_PWORLD 508
;		strvar x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
;		NEW_NPC 1 333030 @x @y 0 zyzqgotozc.mac
		pworld_over
		get_task_var 3158 0 8 1 20
		var 20 10
		set_task_var 3158 0 8 @20 1
	
	endif
	
	GET_MAP_NPC_ID 302058 50 70
	EFFECT_ID 0 @50 stopMontion(%d,"294005")
endproc 


proc 6

	get_task_var 3158 8 8 1 Loc1
	if Loc1 = 1
		strvar Map
		GET_PLAYER_MAPINFO 1 Map 1 1 1 
		if Map != 50800
			TASK_FAIL 3158 1
			return
		endif
	endif
	;传出地图任务失败


	get_task_var 3158 0 8 1 20
	if 20 = 1 
		get_task_var 3158 1 8 1 21
		add 21 1
		set_task_var 3158 1 8 @21 1

		if 21 = 1
			effect 0 0 playEffct(38600,19400,"effect\\entity\\9592_16.ent")
		endif
		if 21 = 3
			new_mon 227001 1 38600 19400 0 73 0 0 0
			add_sys_msg_start
			add_sys_msg 击杀怪物，不要让他到达艾斯旁边！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 21 = 15
			effect 0 0 playEffct(36600,20700,"effect\\entity\\9592_16.ent")
		endif
		if 21 = 17
			new_mon 227001 1 36600 20700 0 74 0 100 0
			
			add_sys_msg_start
			add_sys_msg 追杀怪物，不要让他到达艾斯旁边！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 21 = 30
			effect 0 0 playEffct(36600,18200,"effect\\entity\\9592_16.ent")
		endif
		if 21 = 32
			new_mon 227001 1 36600 18200 0 75 0 100 0
			
			add_sys_msg_start
			add_sys_msg 击杀怪物，不要让他到达艾斯旁边！
			set_sys_msg_mode 3 0 0
			msg_info
				
		endif 
		if 21 > 32
			get_task_var 3158 0 8 1 20
			var 20 2
			set_task_var 3158 0 8 @20 1		
		endif

	endif
	
	get_task_var 3158 8 8 1 Loc1
	if Loc1 = 1
		get_pworld_var 0 32 1 30
		if 30 = 1
			TASK_FAIL 3158 1
			add_sys_msg_start
			add_sys_msg 有1只怪物逃跑了，任务失败!
			set_sys_msg_mode 1 0 0
			msg_info
			
			var 30 0
			set_pworld_var 0 32 @30 1
		endif
	endif
		
	
;;;;;;;;;;;;;;;;;;;;;;;;;
	
	get_task_var 3158 2 8 1 22
	if 22 = 1 
		get_task_var 3158 3 8 1 23
		add 23 1
		set_task_var 3158 3 8 @23 1
		switch 23
			case 2
				effect 0 0 playEffct(35700,19400,"effect\\entity\\9592_16.ent")
				effect 0 0 playEffct(36200,19000,"effect\\entity\\9592_16.ent")
				effect 0 0 playEffct(36200,19800,"effect\\entity\\9592_16.ent")
			endcase
			case 4
				
				new_mon 226013 1 35700 19400 0 76 0 100 0

				new_mon 226013 1 36200 19000 0 77 0 100 0

				new_mon 226013 1 36200 19800 0 78 0 100 0
			endcase
			case 6
				effect 0 0 playEffct(35700,19400,"effect\\entity\\9592_16.ent")
				effect 0 0 playEffct(36200,19000,"effect\\entity\\9592_16.ent")
				effect 0 0 playEffct(36200,19800,"effect\\entity\\9592_16.ent")		
			endcase
			case 8
				new_mon 226013 1 35700 19400 0 76 0 100 0
			
				new_mon 226013 1 36200 19000 0 77 0 100 0
			
				new_mon 226013 1 36200 19800 0 78 0 100 0
			endcase
		endswitch
		if 23 > 8
			get_task_var 3158 2 8 1 22
			var 22 2
			set_task_var 3158 2 8 @22 1
		endif
	endif 
	
	get_task_var 3158 8 8 1 Loc1
	if Loc1 = 1
		get_pworld_var 5 8 1 32
		if 32 = 6
			get_pworld_var 7 8 1 34
			if 34 < 3
				task_fini 3158 1
				
				add_sys_msg_start
				add_sys_msg 你成功通过了这次试炼，任务完成！
				set_sys_msg_mode 1 0 0
				msg_info
				
				GET_MAP_NPC_ID 302058 50 0 
				mon_speech @50 0 2 1 很好，我仿佛看到了未来一位强大的法师，你成功通过试炼了。	
				EFFECT_ID 0 @50 stopMontion(%d,"294005")
				
				get_pworld_var 4 8 1 31			
				var 31 4
				set_pworld_var 4 8 @31 1
			endif
		endif
		
		get_pworld_var 7 8 1 34
		if 34 >= 3
			TASK_FAIL 3158 1
			
			add_sys_msg_start
			add_sys_msg 有$@34$只怪物逃跑了，任务失败!
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
	
endproc 














