

proc 1
    GET_PLAYER_MAPINFO 0 10 11 12 13
	if 10 != 50400
;		RESET_PWORLD 504
		
		ENTER_PWORLD 504 0 30
		;if 30 = 0
		;	dec_item_num_inpak 5300420 1 0
		;endif
		strvar Pworldvar
		get_pworld_var 4 8 1 Pworldvar
		var Pworldvar 10
		set_pworld_var 4 8 @Pworldvar 1		
	else
		add_sys_msg_start
		add_sys_msg 你已经在雷霆崖，不能使用树灵的召唤！
		set_sys_msg_mode 3 0 0
		msg_info

	endif
endproc


