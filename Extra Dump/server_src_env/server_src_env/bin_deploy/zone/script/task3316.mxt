
proc 1
	var 10 0
	set_task_var 3316 0 8 @10 1
	var 11 0
	set_task_var 3316 1 8 @11 1
	var 12 0
	set_task_var 3316 2 8 @12 1
	var 13 0
	set_task_var 3316 3 8 @13 1


endproc


proc 4
 ;   DEL_ROLE_TMP_ITEM  0 0
	get_player_id 1 10
	TARGET_ADD_STATUS 1 @10 3171 1 0 0
endproc 
proc 5
;	DEL_ROLE_TMP_ITEM  0 0
	get_player_id 1 10
	TARGET_ADD_STATUS 1 @10 3171 1 0 0
endproc 

proc 6

	HAS_STATUS 3171 20
	

		
	if 20 = -1
	
		TASK_FAIL 3316 13
	
		
		return
	
	endif

	strvar x y m1 m2 m3 map m4 lv
	GET_PLAYER_MAPINFO 0 map 0 x y
	GET_PLAYER_BASE_ATT 1 1 lv 1
	if map = 4400
		GET_POINT_DIST @x @y 25600 20300 m1
		if m1 < 800
			get_task_var 3316 0 8 1 10
			if 10 = 0
				
				add_sys_msg_start
				add_sys_msg 这条桥又长又窄，好危险的样子，还是让保镖走在前面吧！
				set_sys_msg_mode 3 0 0
				msg_info
				NEW_LIFE_MON 241020 1 25600 20300 0 0 0 0 180 0 0
			;	NEW_LIFE_DYN_MON 1 241020 1 25600 20300 0 0 0 0 0 @lv 0 180
				var 10 1
				set_task_var 3316 0 8 @10 1
			endif
		endif
		
		GET_POINT_DIST @x @y 26100 19000 m2
		if m2 < 800
			get_task_var 3316 1 8 1 11
			if 11 = 0
				add_sys_msg_start
				add_sys_msg 为什么我又有挪不动脚步的感觉呢？
				set_sys_msg_mode 3 0 0
				msg_info
				NEW_LIFE_MON 241020 1 26100 19000 0 0 0 0 180 0 0
			;	NEW_LIFE_DYN_MON 1 241020 1 26100 19000 0 0 0 0 0 @lv 0 180 
				var 11 1
				set_task_var 3316 1 8 @11 1
			endif
		endif
		
		GET_POINT_DIST @x @y 27000 16200 m3
		if m3 < 800
			get_task_var 3316 2 8 1 12
			if 12 = 0
				add_sys_msg_start
				add_sys_msg 为什么我一看见狼就挪不动脚步呢？
				set_sys_msg_mode 3 0 0
				msg_info
				NEW_LIFE_MON 241020 1 27000 16200 0 0 0 0 180 0 0
			;	NEW_LIFE_DYN_MON 1 241020 1 27000 16200 0 0 0 0 0 @lv 0 180 
				var 12 1
				set_task_var 3316 2 8 @12 1
			endif
		endif
		
		GET_POINT_DIST @x @y 27000 14300 m4
		if m4 < 800
			get_task_var 3316 3 8 1 13
			if 13 = 0
				add_sys_msg_start
				add_sys_msg 还真是穷追不舍啊，我都已经快要看到小娟了！
				set_sys_msg_mode 3 0 0
				msg_info
				NEW_LIFE_MON 241020 1 27000 14300 0 0 0 0 180 0 0
			;	NEW_LIFE_DYN_MON 1 241020 1 27200 15000 0 0 0 0 0 @lv 0 180 
				var 13 1
				set_task_var 3316 3 8 @13 1
			endif
		endif
	endif
endproc  








