;蓝蛋 龙蛋定时
proc 1
    var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 4
	
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1
	
	;龙蛋破裂，产生迅猛龙
	if 10 = 20
	DESTROY_MON 1
	get_mon_info 1 283003 0 0 0 0 100 101
	new_mon 283013 1 @100 @101 5 0 0 0 1
	endif
	
	
endproc


