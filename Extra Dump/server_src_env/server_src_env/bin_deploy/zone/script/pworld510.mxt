proc 1
	strvar Com Tim1 Tim2 Loc3 Tim3 Loc4 Tim4
	var 50 0
	set_pworld_var 0 8 @50 1
	var 51 0
	set_pworld_var 1 32 @51 1
	var 52 0
	set_pworld_var 5 8 @52 1
	strvar Com
	var Com 0
	set_pworld_var 6 8 @Com 1

	var 53 0
	set_pworld_var 7 8 @53 1
	
	var Tim1 0
	set_pworld_var 8 8 @Tim1 1
	var Tim2 0
	set_pworld_var 9 8 @Tim2 1
;;;;;;;;;;;;
	var Loc3 0
	set_pworld_var 11 8 @Loc3 1
	var Tim3 0
	set_pworld_var 12 8 @Tim3 1
;;;;;;;;;;
	var Loc4 0
	set_pworld_var 13 8 @Loc4 1
	var Tim4 0
	set_pworld_var 14 8 @Tim4 1
;;;;;;;;;;	
;;;;;;;;;;
	strvar Loc_1 Loc_2
	var Loc_1 0
	set_pworld_var 15 8 @Loc_1 1
	var Loc_2 0
	set_pworld_var 16 8 @Loc_2 1
;;;;;;;;;;;;
	strvar Loc_3 Loc_4
	var Loc_3 0
	set_pworld_var 17 8 @Loc_3 1
	var Loc_4 0
	set_pworld_var 18 8 @Loc_4 1
;;;;;;;;;;;;
	strvar Loc_5 Loc_6
	var Loc_5 0
	set_pworld_var 19 8 @Loc_5 1
	var Loc_6 0
	set_pworld_var 20 8 @Loc_6 1
;;;;;;;;;;;;
	strvar Loc_7 
	var Loc_7 0
	set_pworld_var 21 8 @Loc_7 1
;;;;;;;;
	strvar MapLoc LosTim
	var MapLoc 0
	set_pworld_var 22 8 @MapLoc 1
	var LosTim 0
	set_pworld_var 23 8 @LosTim 1
;;;;;;;;;;
	strvar Num
	var Num 0
	set_pworld_var 24 8 @Num 1
;;;;;;;;;;;;;;;;;
	strvar TimLei
	var TimLei 1
	set_pworld_var 25 8 @TimLei 1	
;;;;;;;;;;;雷蒙1实例
	strvar LeiMid1
	var LeiMid1 0
	set_pworld_var 26 32 @LeiMid1 1

;;;;;;;;;;;雷蒙2实例结盟
	strvar LeiMid2
	var LeiMid2 0
	set_pworld_var 30 32 @LeiMid2 1
	
;;;;;;;;;;;雷蒙3实例不结盟
	strvar LeiMid3
	var LeiMid3 0
	set_pworld_var 34 32 @LeiMid3 1
	
;;;;;;;;;;;信念处雷蒙1
	strvar LeiMid4
	var LeiMid4 0
	set_pworld_var 38 32 @LeiMid4 1
;;;;;;;;;;;信念处雷蒙2
	strvar LeiMid5
	var LeiMid5 0
	set_pworld_var 42 32 @LeiMid5 1
	
		
	add_sys_msg_start
	add_sys_msg 有个空灵的声音在你耳边响起：
	set_sys_msg_mode 3 0 0
	map_msg_info 
endproc 

proc 3
	get_pworld_var 8 8 1 Tim1
	if Tim1 = 0
		get_pworld_var 9 8 1 Tim2
		add Tim2 1
		set_pworld_var 9 8 @Tim2 1
		
		if Tim2 = 3
			add_sys_msg_start
			add_sys_msg 四处走动一下，在这白茫茫的世界中寻找雷蒙盖顿。
			set_sys_msg_mode 3 0 0
			map_msg_info 
			
			var Tim2 0
			set_pworld_var 9 8 @Tim2 1
			
		;	var Tim1 1
		;	set_pworld_var 8 8 @Tim1 1
		endif
	endif
	
	strvar Num
	GET_PWORLD_ROLE_NUM Num  1
	if Num = 0
		COMPEL_CLOSE_PWORLD 1
	endif
	
endproc 


