
proc 1
	strvar So So1 So2 So3
	var So 0
	set_task_var 3114 0 8 @So 1
	var So3 0
	set_task_var 3114 3 8 @So3 1


	var So1 0
	set_task_var 3114 1 8 @So1 1
	var So2 0
	set_task_var 3114 2 8 @So2 1
	
	strvar Sot
	var Sot 0
	set_pworld_var 11 8 @Sot 1
	
endproc 

proc 2
	
	get_task_var 3114 1 8 1 So1 
	var So1 1
	set_task_var 3114 1 8 @So1 1
	
	
endproc

proc 5
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
	
	pworld_over
	COMPEL_CLOSE_PWORLD 1
	
	strvar Sot
	get_pworld_var 11 8 1 Sot
	var Sot 10
	set_pworld_var 11 8 @Sot 1
	
	DESTROY_MON_BY_DEFID 1 202005 
	DESTROY_MON_BY_DEFID 1 202006 
	
	
;	OUT_PWORLD_BY_SINGLE
;	RESET_PWORLD 504
	
endproc 

proc 7
	
	get_task_var 3114 1 8 1 So1 
	if So1 = 1
		
		get_task_var 3114 2 8 1 So2
		add So2 1
		set_task_var 3114 2 8 @So2 1
		if So2 = 2
;			OUT_PWORLD_BY_SINGLE
			new_npc 1 333020 7779 8542 0 jlgozc.mac
			
		endif
		if So2 = 5
			strvar gout
			get_pworld_var 12 8 1 gout
			var gout 1
			set_pworld_var 12 8 @gout 1
			
			var So1 0
			set_task_var 3114 1 8 So1 1
		endif
		
	endif
endproc 

