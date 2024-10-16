;5号BOSS

proc 2
	strvar hp
	GET_MON_INFO 0 0 0 hp 0 0 0 0
	if hp = 0
		
	;	add_sys_msg_start
	;	add_sys_msg 牢笼老大大喊：这更不科学了！！！也嗝屁了~~~~~~
	;	set_sys_msg_mode 4 0 0
	;	map_msg_info
		
		get_pworld_var 102 8 1 42
		var 42 1
		set_pworld_var 102 8 @42 1
		
	else
		get_pworld_var 102 8 1 42
		var 42 2
		set_pworld_var 102 8 @42 1
	endif
endproc 

proc 9

	DESTROY_MON 1
	strvar momid
	NEW_MON_BY_DIR 205122 1 17000 14000 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 

endproc 

