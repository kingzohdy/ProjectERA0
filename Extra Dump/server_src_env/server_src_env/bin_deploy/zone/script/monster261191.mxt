;公会守护枢纽活动    ,枢纽怪
proc 1
	;monsay10,枢纽怪被打喊疼
	var 10 0
	set_mon_var 0 32 @10 1
	;monhp11,枢纽怪上个tick的血量
	var 11 0
	set_mon_var 4 32 @11 1
	GET_MON_INFO 1 0 10 0 0 0 0 0 
	;monmemid10,枢纽怪memid
	
	set_pworld_var 0 32 @10 1
	var 11 1
	;monstate11,枢纽怪状态 0未刷 1存活 2死亡
	
	set_pworld_var 4 32 @11 1
endproc
proc 2
	var 11 2
	;monstate11,枢纽怪状态 0未刷 1存活 2死亡
	
	set_pworld_var 4 32 @11 1
endproc
;tick
proc 4
	strvar hp
	GET_MON_INFO 1 0 10 hp 0 0 0 0 
	;monmemid10,枢纽怪memid
	
	set_pworld_var 0 32 @10 1
	;monsay10,枢纽怪被打喊疼
	var 10 0
	get_mon_var 0 32 1 10
	;monhp11,枢纽怪上个tick的血量
	var 11 0
	get_mon_var 4 32 1 11
	if 11 > @hp
		var 20 @10
		mod 20 7
		if 20 = 0
			;提示
			add_sys_msg_start
			add_sys_msg $_Monster$正受到敌方攻击，请尽快支援 
			set_sys_msg_mode 14 0 0
			map_msg_info
		endif
		add 10 1
		;monsay10,枢纽怪被打喊疼
		
		set_mon_var 0 32 @10 1
	endif
	var 11 @hp
	;monhp11,枢纽怪上个tick的血量
	
	set_mon_var 4 32 @11 1
endproc
