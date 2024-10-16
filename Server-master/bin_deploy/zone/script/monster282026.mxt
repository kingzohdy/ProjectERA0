;哈托尔


proc 2
	strvar Loc_3 Loc4
;	GET_MON_INFO 1 1 1 1 1 1 X Y
	EFFECT_ID 0 0 setOverlayRegion("区域_4")
;	NEW_MON 282020 1 6000 6600 0 0 0 0 0
		add_sys_msg_start
		add_sys_msg 更大的挑战即将来临，向前走，击败你所遇见的怪物！
		set_sys_msg_mode 3 0 0
		map_msg_info 
	
	get_pworld_var 17 8 1 Loc_3
	var Loc_3 1
	set_pworld_var 17 8 @Loc_3 1
	
	get_pworld_var 13 8 1 Loc4
	var Loc4 0
	set_pworld_var 13 8 @Loc4 1
endproc 

