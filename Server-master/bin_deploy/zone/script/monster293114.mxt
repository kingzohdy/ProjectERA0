;÷ÿ…À…Ω—Ú


proc 2
	get_pworld_var 8 32 1 61
	add 61 1
	set_pworld_var 8 32 @61 1
endproc 

proc 4

	get_pworld_var 4 32 1 60
	if 60 = 0
		return
	endif


	get_mon_var 0 8 1 11
	add 11 1
	set_mon_var 0 8 @11 1
	
	
	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	
	if 11 = 2
;		DESTROY_MON 1
		MON_SET_HP -600 @Mid 1
		var 11 0
		set_mon_var 0 8 @11 1
	endif



endproc 


