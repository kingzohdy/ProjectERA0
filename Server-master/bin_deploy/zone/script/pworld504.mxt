
proc 1

	var 10 0
	set_pworld_var 0 32 @10 1
	
	strvar Pworldvar
	var Pworldvar 0
	set_pworld_var 4 8 @Pworldvar 1
	
	
	strvar Num
	var Num 0
	set_pworld_var 5 8 @Num 1
	
	strvar Num_1 Num_2
	var Num_1 0
	set_pworld_var 6 8 @Num_1 1
	var Num_2 0
	set_pworld_var 7 8 @Num_2 1
	
	strvar NumT1 NumT2
	var NumT1 0
	set_pworld_var 9 8 @NumT1 1
	var NumT2 0
	set_pworld_var 10 8 @NumT2 1
	
	
	effect 0 0 playEffct(7700,5000,"effect\\entity\\9570_16.ent")
	strvar Sot
	var Sot 0
	set_pworld_var 11 8 @Sot 1
	
	strvar gout
	var gout 0
	set_pworld_var 12 8 @gout 1
	
endproc 

proc 3

	strvar Num
	GET_PWORLD_ROLE_NUM Num 1
	if Num = 0
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
		return
	endif

	get_pworld_var 12 8 1 gout
	if gout = 1
		PWORLD_OVER
		COMPEL_CLOSE_PWORLD 1
		return
	endif

;	COMPEL_CLOSE_PWORLD 1
;;;;10秒后招唤第一波
	strvar NumT1 
	get_pworld_var 9 8 1 NumT1
	if NumT1 = 1
		get_pworld_var 10 8 1 NumT2
		add NumT2 1
		set_pworld_var 10 8 @NumT2 1

		if NumT2 = 5
			
			CIRCLE_NEW_MON 202006 10 7779 8542 500 1 10 0 0 0 0 0 0
			var NumT1 0
			set_pworld_var 9 8 @NumT1 1
			add_sys_msg_start
			add_sys_msg 点击右下角的精灵技能图标，变身为精灵，按1使用精灵技能
			set_sys_msg_mode 1 0 0
			map_msg_info
			
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Num Num_1 Num_2
	get_pworld_var 5 8 1 Num
	if Num <= 10
		if Num = 10
;			NEW_TMP_ITEM 1440240 7779 8542 0 0 0 0
;			CIRCLE_NEW_MON 202006 10 7779 8542 500 1 10 0 0 0 0 0 0
			var Num 30
			set_pworld_var 5 8 @Num 1 
			
;			add_sys_msg_start
;			add_sys_msg 拾取红色的精灵果
;			set_sys_msg_mode 1 0 0
;			map_msg_info
			
			set_pworld_var 6 8 1 Num_1 
			var Num_1 1
			set_pworld_var 6 8 @Num_1 1
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	向下的箭头
	get_pworld_var 6 8 1 Num_1
	if Num_1 = 0
;	
		get_pworld_var 7 8 1 Num_2 
		add Num_2 1
		set_pworld_var 7 8 @Num_2 1
		
		if Num_2 >= 2
		
			effect 0 0 playEffct(7700,5000,"effect\\entity\\9570_16.ent")
			set_pworld_var 7 8 1 Num_2
			var Num_2 0
			set_pworld_var 7 8 @Num_2 1
			
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	get_pworld_var 6 8 1 Num_1
	if Num_1 = 10
		get_pworld_var 7 8 1 Num_2 
		add Num_2 1
		set_pworld_var 7 8 @Num_2 1
		if Num_2 >= 2
			
			effect 0 0 playEffct(7900,7600,270,"effect\\entity\\9570_16.ent")
			set_pworld_var 7 8 1 Num_2
			var Num_2 0
			set_pworld_var 7 8 @Num_2 1
		endif
	endif
endproc 
