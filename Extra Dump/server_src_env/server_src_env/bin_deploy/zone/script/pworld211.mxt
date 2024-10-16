;龙蛋副本
;暂定第一波怪10秒后开始刷，其余一分钟后开始刷下一波



proc 1

;10据上次刷怪后开始计时，11怪物波次计数,18普通迅猛龙个数，19毒迅猛龙个数，20蓝迅猛龙个数，22臭迅猛龙个数，23迅猛龙总数
	var 10 0
	set_pworld_var 0 32 @10 1
	
	var 11 1
	set_pworld_var 4 32 @11 1
	var 12 0
	set_pworld_var 16 32 @12 1
	var 13 0
	set_pworld_var 12 32 @13 1
	var 14 0
	set_pworld_var 20 32 @14 1
	var 15 0
	set_pworld_var 24 32 @15 1
	var 16 0
	set_pworld_var 28 32 @16 1
	var 17 0
	set_pworld_var 32 32 @17 1
	var 21 0
	set_pworld_var 8 32 @21 1
	var 18 0
	set_pworld_var 36 32 @18 1
	var 19 0
	set_pworld_var 40 32 @19 1
	var 20 0
	set_pworld_var 44 32 @20 1
	var 22 0
	set_pworld_var 48 32 @22 1
	var 23 0
	set_pworld_var 52 32 @23 1
	;;;;;;;;;;;;

	
	var 24 0
	set_pworld_var 56 8 @24 1
	var 25 0
	set_pworld_var 57 32 @25 1
endproc

proc 3
	GET_MON_NUM 1 14
	set_pworld_var 20 32 @14 1
	get_mon_num_by_defid 283010 18
	get_mon_num_by_defid 283011 19
	get_mon_num_by_defid 283012 20
	get_mon_num_by_defid 283013 22
	set_pworld_var 36 32 @18 1
	set_pworld_var 40 32 @19 1
	set_pworld_var 44 32 @20 1
	set_pworld_var 48 32 @22 1
	get_pworld_var 36 32 1 18
	get_pworld_var 40 32 1 19
	get_pworld_var 44 32 1 20
	get_pworld_var 48 32 1 22
	var 23 0
	add 23 @18
	add 23 @19
	add 23 @20
	add 23 @22
	set_pworld_var 52 32 @23 1
	
	;计时器
	get_pworld_var 0 32 1 10
	add 10 1
	set_pworld_var 0 32 @10 1

	get_pworld_var 0 32 1 10
	get_pworld_var 4 32 1 11
	get_pworld_var 16 32 1 12
	get_pworld_var 12 32 1 13
	get_pworld_var 20 32 1 14
	get_pworld_var 32 32 1 17
	get_pworld_var 36 32 1 18
	get_pworld_var 40 32 1 19
	get_pworld_var 44 32 1 20
	get_pworld_var 8 32 1 21
	get_pworld_var 48 32 1 22
	;get_pworld_var 52 32 1 23
	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 11=$@11$ 12=$@12$ 14=$@14$ 18=$@18$ 19=$@19$ 20=$@20$ 21=$@21$ 22=$@22$ 23=$@23$
	;SET_SYS_MSG_MODE 0 0 0
	;MAP_MSG_INFO
	
	;if 23 <= 20
		;var 23 0
		;set_pworld_var 52 32 @23 1
	;endif

	if 23 > 20 and 23 < 30
		ADD_SYS_MSG_START
		ADD_SYS_MSG 警告！警告！迅猛龙过多，时空通道开始不稳定了
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
		;var 23 0
		;set_pworld_var 52 32 @23 1
	endif
	
	if 23 >= 30
		var 11 11
		set_pworld_var 4 32 @11 1
		;var 23 0
		;set_pworld_var 52 32 @23 1
		new_npc 1 333003 6568 6460 0 zhu30out.mac
		ADD_SYS_MSG_START
		ADD_SYS_MSG 迅猛龙过多，时空通道关闭，挑战失败，可通过传送门离开
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
		pworld_over
	endif
	

	switch 11
		case 1
		;准备阶段
			if 10 = 2
				ADD_SYS_MSG_START
				ADD_SYS_MSG 霸王龙：孩子们，午餐时刻到了。。。
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endif

			;if 10 = 5
			if 10 >= 5
				add_sys_msg_start
				add_sys_msg 第一阶段龙蛋，将在15秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
			endif


		endcase
		
		case 2
		;刷第一批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 1 and 10 < 16
			var 10 16
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 2 and 10 < 19
			var 10 19
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 3 and 10 < 22
			var 10 22
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 4 and 10 < 25
			var 10 25
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 5 and 10 < 28
			var 10 28
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 6 and 10 < 31
			var 10 31
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 7 and 10 < 34
			var 10 34
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 8 and 10 < 37
			var 10 37
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 9 and 10 < 40
			var 10 40
			set_pworld_var 0 32 @10 1
			endif

			get_pworld_var 0 32 1 10
			if 10 = 8
				add_sys_msg_start
				add_sys_msg 第一阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info

			endif

			if 10 = 13
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
	
			if 10 = 16
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 19
				new_mon 283001 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 22
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 25
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 28
				new_mon 283002 1 6800 5067 800 0 0 0 2
			endif
	
			if 10 = 31
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 34
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 37
				new_mon 283000 1 6800 5067 800 0 0 0 2
			endif
			
			if 10 = 40
				new_mon 283003 1 6800 5067 800 0 0 0 2
			endif
			
			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 40 and 14 = 0 and @21 < 10
				var 21 10
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 10 and 17 = 1 and 14 = 0
				var 21 11
				set_pworld_var 8 32 @21 1
			endif

			
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 10 and 17 = 0
				new_mon 283004 1 6800 5067 0 0 0 0 2
				var 17 1
				set_pworld_var 32 32 @17 1
			endif

			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 11 and 17 = 1
				var 17 2
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 2 and 10 >= 2
				add_sys_msg_start
				add_sys_msg 祝贺大家通过第一阶段，第二阶段挑战将于20秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif

		endcase
		
		case 3
		;第二批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 13 and 10 < 15
			var 10 15
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 15 and 10 < 20
			var 10 20
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 17 and 10 < 25
			var 10 25
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 19 and 10 < 30
			var 10 30
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 21 and 10 < 35
			var 10 35
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 23 and 10 < 40
			var 10 40
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 25 and 10 < 45
			var 10 45
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 27 and 10 < 50
			var 10 50
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 29 and 10 < 55
			var 10 55
			set_pworld_var 0 32 @10 1
			endif

			get_pworld_var 0 32 1 10
			if 10 = 5
				add_sys_msg_start
				add_sys_msg 第二阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_tmp_item 1420001 7500 6067 0 0 30 2
			endif

			if 10 = 10
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 2 7500 6067 800 0 0 0 2
			endif
	
			if 10 = 15
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283001 1 7500 6067 800 0 0 0 2
			endif
			
			if 10 = 20
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283002 1 7500 6067 800 0 0 0 2
			endif
			
			if 10 = 25
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283003 1 7500 6067 800 0 0 0 2
			endif
	
			if 10 = 30
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283001 1 7500 6067 800 0 0 0 2
			endif
			
			if 10 = 35
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283002 1 7500 6067 800 0 0 0 2
			endif
			
			if 10 = 40
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283003 1 7500 6067 800 0 0 0 2
			endif

			if 10 = 45
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283001 1 7500 6067 800 0 0 0 2
			endif
			
			if 10 = 50
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283002 1 7500 6067 800 0 0 0 2
			endif

			if 10 = 55
				new_mon 283000 1 7500 6067 800 0 0 0 2
				new_mon 283003 1 7500 6067 800 0 0 0 2
			endif

			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 55 and 14 = 0 and @21 < 31
				var 21 31
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 31 and 17 = 3 and 14 = 0
				var 21 32
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 31 and 17 = 2
				new_mon 283005 1 7500 6067 0 0 0 0 2
				var 17 3
				set_pworld_var 32 32 @17 1
			endif

			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 32 and 17 = 3
				var 17 4
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 4 and 10 >= 2

				add_sys_msg_start
				add_sys_msg 祝贺大家通过第二阶段，第三阶段挑战将于20秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif
			
		endcase
	
		case 4
		;第三批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 36 and 10 < 18
			var 10 18
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 40 and 10 < 26
			var 10 26
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 44 and 10 < 34
			var 10 34
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 48 and 10 < 42
			var 10 42
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 52 and 10 < 50
			var 10 50
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 56 and 10 < 58
			var 10 58
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 60 and 10 < 66
			var 10 66
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 64 and 10 < 74
			var 10 74
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 68 and 10 < 82
			var 10 82
			set_pworld_var 0 32 @10 1
			endif

			get_pworld_var 0 32 1 10
			if 10 = 5
				add_sys_msg_start
				add_sys_msg 第三阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_tmp_item 1400000 6800 6667 0 0 30 2
			endif

			if 10 = 10
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 4 6800 6667 300 0 0 0 2
			endif
	
			if 10 = 18
				new_mon 283000 4 6800 7667 300 0 0 0 2
			endif
			
			if 10 = 26
				new_mon 283000 3 6800 6667 300 0 0 0 2
				new_mon 283001 1 6800 6667 300 0 0 0 2
			endif
			
			if 10 = 34
				new_mon 283000 3 6800 7667 300 0 0 0 2
				new_mon 283002 1 6800 7667 300 0 0 0 2
			endif
	
			if 10 = 42
				new_mon 283000 3 6800 6667 300 0 0 0 2
				new_mon 283003 1 6800 6667 300 0 0 0 2
			endif
			
			if 10 = 50
				new_mon 283000 2 6800 7667 300 0 0 0 2
				new_mon 283001 1 6800 7667 300 0 0 0 2
				new_mon 283002 1 6800 7667 300 0 0 0 2
			endif
			
			if 10 = 58
				new_mon 283000 2 6800 6667 300 0 0 0 2
				new_mon 283002 1 6800 6667 300 0 0 0 2
				new_mon 283003 1 6800 6667 300 0 0 0 2
			endif
			
			if 10 = 66
				new_mon 283000 2 6800 7667 300 0 0 0 2
				new_mon 283001 1 6800 7667 300 0 0 0 2
				new_mon 283003 1 6800 7667 300 0 0 0 2
			endif
			
			if 10 = 74
				new_mon 283000 1 6800 6667 300 0 0 0 2
				new_mon 283001 1 6800 6667 300 0 0 0 2
				new_mon 283002 1 6800 6667 300 0 0 0 2
				new_mon 283003 1 6800 6667 300 0 0 0 2
			endif
			if 10 = 82
				new_mon 283000 1 6800 7667 300 0 0 0 2
				new_mon 283001 1 6800 7667 300 0 0 0 2
				new_mon 283002 1 6800 7667 300 0 0 0 2
				new_mon 283003 1 6800 7667 300 0 0 0 2
			endif
			
			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 82 and 14 = 0 and @21 < 72
				var 21 72
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 72 and 17 = 5 and 14 = 0
				var 21 73
				set_pworld_var 8 32 @21 1
			endif


			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 72 and 17 = 4
				new_mon 283006 1 6800 7167 0 0 0 0 2
				var 17 5
				set_pworld_var 32 32 @17 1
			endif

			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 73 and 17 = 5
				var 17 6
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 6 and 10 >= 2

				add_sys_msg_start
				add_sys_msg 祝贺大家通过第三阶段，第四阶段挑战将于20秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif

		endcase
	
	
		case 5
		;第四批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 79 and 10 < 18
			var 10 18
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 85 and 10 < 26
			var 10 26
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 91 and 10 < 34
			var 10 34
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 97 and 10 < 42
			var 10 42
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 103 and 10 < 50
			var 10 50
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 109 and 10 < 58
			var 10 58
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 115 and 10 < 66
			var 10 66
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 121 and 10 < 74
			var 10 74
			set_pworld_var 0 32 @10 1
			endif
			if 21 = 127 and 10 < 82
			var 10 82
			set_pworld_var 0 32 @10 1
			endif

			get_pworld_var 0 32 1 10
			if 10 = 5
				add_sys_msg_start
				add_sys_msg 第四阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				;new_tmp_item 1400000 5500 7367 0 0 30 1
				new_tmp_item 1420001 5700 7067 0 0 30 1
			endif

			if 10 = 10
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 3 0 0
				map_msg_info
				new_mon 283000 6 5200 6667 500 0 0 0 2
			endif
	
			if 10 = 18
				new_mon 283000 5 6200 6667 500 0 0 0 2
				new_mon 283001 1 6200 6667 500 0 0 0 2
			endif
			
			if 10 = 26
				new_mon 283000 5 5500 6567 500 0 0 0 2
				new_mon 283002 1 5500 6567 500 0 0 0 2
			endif
			
			if 10 = 34
				new_mon 283000 5 5200 6667 500 0 0 0 2
				new_mon 283003 1 5200 6667 500 0 0 0 2
			endif
	
			if 10 = 42
				new_mon 283000 4 6200 6667 500 0 0 0 2
				new_mon 283001 1 6200 6667 500 0 0 0 2
				new_mon 283002 1 6200 6667 500 0 0 0 2
			endif
			
			if 10 = 50
				new_mon 283000 4 5500 6567 500 0 0 0 2
				new_mon 283001 1 5500 6567 500 0 0 0 2
				new_mon 283003 1 5500 6567 500 0 0 0 2
			endif
			
			if 10 = 58
				new_mon 283000 4 5200 6667 500 0 0 0 2
				new_mon 283002 1 5200 6667 500 0 0 0 2
				new_mon 283003 1 5200 6667 500 0 0 0 2
			endif
			
			if 10 = 66
				new_mon 283000 6 6200 6667 500 0 0 0 2
			endif
			
			if 10 = 74
				new_mon 283000 3 5500 6567 500 0 0 0 2
				new_mon 283001 1 5500 6567 500 0 0 0 2
				new_mon 283002 1 5500 6567 500 0 0 0 2
				new_mon 283003 1 5500 6567 500 0 0 0 2
			endif
			if 10 = 82
				new_mon 283000 3 5200 6667 500 0 0 0 2
				new_mon 283001 1 5200 6667 500 0 0 0 2
				new_mon 283002 1 5200 6667 500 0 0 0 2
				new_mon 283003 1 5200 6667 500 0 0 0 2
			endif

			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 82 and 14 = 0 and @21 < 133
				var 21 133
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 133 and 17 = 7 and 14 = 0
				var 21 134
				set_pworld_var 8 32 @21 1
			endif

			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 133 and 17 = 6
				new_mon 283007 1 5500 7167 0 0 0 0 2
				var 17 7
				set_pworld_var 32 32 @17 1
			endif

			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 134 and 17 = 7
				var 17 8
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 8 and 10 >= 2
				add_sys_msg_start
				add_sys_msg 祝贺大家通过第四阶段，第五阶段挑战将于30秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif

		endcase	
	
	
	
		case 6
		;第五批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 142 and 10 < 30
			var 10 30
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 150 and 10 < 45
			var 10 45
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 158 and 10 < 60
			var 10 60
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 166 and 10 < 75
			var 10 75
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 174 and 10 < 90
			var 10 90
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 182 and 10 < 105
			var 10 105
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 190 and 10 < 120
			var 10 120
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 198 and 10 < 135
			var 10 135
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 206 and 10 < 150
			var 10 150
			set_pworld_var 0 32 @10 1
			endif
			
			get_pworld_var 0 32 1 10
			if 10 = 10
				add_sys_msg_start
				add_sys_msg 第五阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_tmp_item 1400000 4800 6267 0 0 30 1
				;new_tmp_item 1420001 5200 5867 0 0 30 1
			endif


			if 10 = 15
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 6 4500 6367 600 0 0 0 2
				new_mon 283001 1 4500 6367 600 0 0 0 2
				new_mon 283002 1 4500 6367 550 0 0 0 2
			endif
	
			if 10 = 30
				new_mon 283000 6 5300 6067 600 0 0 0 2
				new_mon 283001 1 5300 6067 600 0 0 0 2
				new_mon 283003 1 5300 6067 600 0 0 0 2
			endif
			
			if 10 = 45
				new_mon 283000 6 5000 5467 600 0 0 0 2
				new_mon 283002 1 5000 5467 600 0 0 0 2
				new_mon 283003 1 5000 5467 600 0 0 0 2
			endif
			
			if 10 = 60
				new_mon 283000 4 4500 6367 600 0 0 0 2
				new_mon 283001 2 4500 6367 600 0 0 0 2
				new_mon 283003 2 4500 6367 600 0 0 0 2
			endif
	
			if 10 = 75
				new_mon 283000 4 5300 6067 600 0 0 0 2
				new_mon 283002 2 5300 6067 600 0 0 0 2
				new_mon 283003 2 5300 6067 600 0 0 0 2
			endif
			
			if 10 = 90
				new_mon 283000 4 5000 5467 600 0 0 0 2
				new_mon 283001 2 5000 5467 600 0 0 0 2
				new_mon 283002 2 5000 5467 600 0 0 0 2
			endif
			
			if 10 = 105
				new_mon 283000 5 4500 6367 600 0 0 0 2
				new_mon 283001 1 4500 6367 600 0 0 0 2
				new_mon 283002 1 4500 6367 600 0 0 0 2
				new_mon 283003 1 4500 6367 600 0 0 0 2
			endif
			
			if 10 = 120
				new_mon 283000 5 5300 6067 600 0 0 0 2
				new_mon 283001 1 5300 6067 600 0 0 0 2
				new_mon 283002 1 5300 6067 600 0 0 0 2
				new_mon 283003 1 5300 6067 600 0 0 0 2
			endif
			
			if 10 = 135
				new_mon 283000 5 5000 5467 600 0 0 0 2
				new_mon 283001 1 5000 5467 600 0 0 0 2
				new_mon 283002 1 5000 5467 600 0 0 0 2
				new_mon 283003 1 5000 5467 600 0 0 0 2
			endif
			if 10 = 150
				new_mon 283000 5 4500 6367 600 0 0 0 2
				new_mon 283001 1 4500 6367 600 0 0 0 2
				new_mon 283002 1 4500 6367 600 0 0 0 2
				new_mon 283003 1 4500 6367 600 0 0 0 2
			endif

			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 150 and 14 = 0 and @21 < 214
				var 21 214
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 214 and 17 = 9 and 14 = 0
				var 21 215
				set_pworld_var 8 32 @21 1
			endif


			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 214 and 17 = 8
				new_mon 283008 1 5000 6067 0 0 0 0 2
				var 17 9
				set_pworld_var 32 32 @17 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 215 and 17 = 9
				var 17 10
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif

			if 17 = 10 and 10 >= 2
				add_sys_msg_start
				add_sys_msg 祝贺大家通过第五阶段，第六阶段挑战将于30秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif
			

		endcase	
	
		case 7
		;第六批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 224 and 10 < 35
			var 10 35
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 233 and 10 < 55
			var 10 55
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 242 and 10 < 75
			var 10 75
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 251 and 10 < 95
			var 10 95
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 260 and 10 < 115
			var 10 115
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 269 and 10 < 135
			var 10 135
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 278 and 10 < 155
			var 10 155
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 287 and 10 < 175
			var 10 175
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 296 and 10 < 195
			var 10 195
			set_pworld_var 0 32 @10 1
			endif
			
			if 21 = 305 and 10 < 215
			var 10 215
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 314 and 10 < 235
			var 10 235
			set_pworld_var 0 32 @10 1
			endif
			


			get_pworld_var 0 32 1 10
			if 10 = 10
				add_sys_msg_start
				add_sys_msg 第六阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				;new_tmp_item 1400000 5300 5267 0 0 30 1
				;new_tmp_item 1440240 5700 4867 0 0 30 1
				new_tmp_item 1420001 5300 4867 0 0 30 1
			endif

			if 10 = 15
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 6 5000 5567 600 0 0 0 2
				new_mon 283001 1 5000 5567 600 0 0 0 2
				new_mon 283002 1 5000 5567 600 0 0 0 2
				new_mon 283003 1 5000 5567 600 0 0 0 2
			endif
	
			if 10 = 35
				new_mon 283000 6 6000 5567 600 0 0 0 2
				new_mon 283001 1 6000 5567 600 0 0 0 2
				new_mon 283002 1 6000 5567 600 0 0 0 2
				new_mon 283003 1 6000 5567 600 0 0 0 2
			endif
			
			if 10 = 55
				new_mon 283000 6 6000 4567 600 0 0 0 2
				new_mon 283001 1 6000 4567 600 0 0 0 2
				new_mon 283002 1 6000 4567 600 0 0 0 2
				new_mon 283003 1 6000 4567 600 0 0 0 2
			endif
			
			if 10 = 75
				new_mon 283000 6 5000 4567 600 0 0 0 2
				new_mon 283001 1 5000 4567 600 0 0 0 2
				new_mon 283002 1 5000 4567 600 0 0 0 2
				new_mon 283003 1 5000 4567 600 0 0 0 2
			endif
	
			if 10 = 95
				new_mon 283000 6 5000 5567 600 0 0 0 2
				new_mon 283001 1 5000 5567 600 0 0 0 2
				new_mon 283002 1 5000 5567 600 0 0 0 2
				new_mon 283003 1 5000 5567 600 0 0 0 2
			endif
			
			if 10 = 115
				new_mon 283000 6 6000 5567 600 0 0 0 2
				new_mon 283001 1 6000 5567 600 0 0 0 2
				new_mon 283002 1 6000 5567 600 0 0 0 2
				new_mon 283003 1 6000 5567 600 0 0 0 2
			endif
			
			if 10 = 135
				new_mon 283000 3 6000 4567 600 0 0 0 2
				new_mon 283001 2 6000 4567 600 0 0 0 2
				new_mon 283002 2 6000 4567 600 0 0 0 2
				new_mon 283003 2 6000 4567 600 0 0 0 2
			endif
			
			if 10 = 155
				new_mon 283000 3 5000 4567 600 0 0 0 2
				new_mon 283001 2 5000 4567 600 0 0 0 2
				new_mon 283002 2 5000 4567 600 0 0 0 2
				new_mon 283003 2 5000 4567 600 0 0 0 2
			endif
			
			if 10 = 175
				new_mon 283000 3 5000 5567 600 0 0 0 2
				new_mon 283001 2 5000 5567 600 0 0 0 2
				new_mon 283002 2 5000 5567 600 0 0 0 2
				new_mon 283003 2 5000 5567 600 0 0 0 2
			endif
			if 10 = 195
				new_mon 283000 3 6000 5567 600 0 0 0 2
				new_mon 283001 2 6000 5567 600 0 0 0 2
				new_mon 283002 2 6000 5567 600 0 0 0 2
				new_mon 283003 2 6000 5567 600 0 0 0 2
			endif
			if 10 = 215
				new_mon 283000 3 6000 4567 600 0 0 0 2
				new_mon 283001 2 6000 4567 600 0 0 0 2
				new_mon 283002 2 6000 4567 600 0 0 0 2
				new_mon 283003 2 6000 4567 600 0 0 0 2
			endif
			if 10 = 235
				new_mon 283000 3 5000 4567 600 0 0 0 2
				new_mon 283001 2 5000 4567 600 0 0 0 2
				new_mon 283002 2 5000 4567 600 0 0 0 2
				new_mon 283003 2 5000 4567 600 0 0 0 2
			endif


			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 235 and 14 = 0 and @21 < 323
				var 21 323
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 323 and 17 = 11 and 14 = 0
				var 21 324
				set_pworld_var 8 32 @21 1
			endif

			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 323 and 17 = 10
				new_mon 283009 1 5500 5067 0 0 0 0 2
				var 17 11
				set_pworld_var 32 32 @17 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 324 and 17 = 11
				var 17 12
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 12 and 10 >= 2

				add_sys_msg_start
				add_sys_msg 祝贺大家通过第六阶段，第七阶段将于30秒后开始
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif

		endcase	

		case 8
		;第七批龙蛋
			;加速器
			get_pworld_var 8 32 1 21
			get_pworld_var 0 32 1 10
			if 21 = 334 and 10 < 40
			var 10 40
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 344 and 10 < 65
			var 10 65
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 354 and 10 < 90
			var 10 90
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 364 and 10 < 115
			var 10 115
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 374 and 10 < 140
			var 10 140
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 384 and 10 < 165
			var 10 165
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 394 and 10 < 190
			var 10 190
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 404 and 10 < 215
			var 10 215
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 414 and 10 < 240
			var 10 240
			set_pworld_var 0 32 @10 1
			endif
			
			if 21 = 424 and 10 < 265
			var 10 265
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 434 and 10 < 290
			var 10 290
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 444 and 10 < 315
			var 10 315
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 454 and 10 < 340
			var 10 340
			set_pworld_var 0 32 @10 1
			endif

			if 21 = 464 and 10 < 365
			var 10 365
			set_pworld_var 0 32 @10 1
			endif




			get_pworld_var 0 32 1 10
			if 10 = 10
				add_sys_msg_start
				add_sys_msg 第七阶段龙蛋，将在10秒后刷新。
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_tmp_item 1400000 6068 6637 0 0 30 1
				;new_tmp_item 1440240 5700 4867 0 0 30 1
				;new_tmp_item 1420001 6668 6637 0 0 30 1
			endif

			if 10 = 15
				add_sys_msg_start
				add_sys_msg 龙蛋出现了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_mon 283000 7 5668 5937 650 0 0 0 2
				new_mon 283001 1 5668 5937 650 0 0 0 2
				new_mon 283002 1 5668 5937 650 0 0 0 2
				new_mon 283003 1 5668 5937 650 0 0 0 2
			endif
	
			if 10 = 40
				new_mon 283000 7 7068 7337 650 0 0 0 2
				new_mon 283001 1 7068 7337 650 0 0 0 2
				new_mon 283002 1 7068 7337 650 0 0 0 2
				new_mon 283003 1 7068 7337 650 0 0 0 2
			endif
			
			if 10 = 65
				new_mon 283000 7 5668 7337 650 0 0 0 2
				new_mon 283001 1 5668 7337 650 0 0 0 2
				new_mon 283002 1 5668 7337 650 0 0 0 2
				new_mon 283003 1 5668 7337 650 0 0 0 2
			endif
			
			if 10 = 90
				new_mon 283000 7 7068 5937 650 0 0 0 2
				new_mon 283001 1 7068 5937 650 0 0 0 2
				new_mon 283002 1 7068 5937 650 0 0 0 2
				new_mon 283003 1 7068 5937 650 0 0 0 2
			endif
	
			if 10 = 115
				new_mon 283000 7 7068 7337 650 0 0 0 2
				new_mon 283001 1 7068 7337 650 0 0 0 2
				new_mon 283002 1 7068 7337 650 0 0 0 2
				new_mon 283003 1 7068 7337 650 0 0 0 2
			endif
			
			if 10 = 140
				new_mon 283000 7 5668 7337 650 0 0 0 2
				new_mon 283001 1 5668 7337 650 0 0 0 2
				new_mon 283002 1 5668 7337 650 0 0 0 2
				new_mon 283003 1 5668 7337 650 0 0 0 2
			endif
			
			if 10 = 165
				new_mon 283000 7 5668 5937 650 0 0 0 2
				new_mon 283001 1 5668 5937 650 0 0 0 2
				new_mon 283002 1 5668 5937 650 0 0 0 2
				new_mon 283003 1 5668 5937 650 0 0 0 2
			endif
			
			if 10 = 190
				new_mon 283000 7 7068 5937 650 0 0 0 2
				new_mon 283001 1 7068 5937 650 0 0 0 2
				new_mon 283002 1 7068 5937 650 0 0 0 2
				new_mon 283003 1 7068 5937 650 0 0 0 2
			endif
			
			if 10 = 215
				new_mon 283000 7 7068 7337 650 0 0 0 2
				new_mon 283001 1 7068 7337 650 0 0 0 2
				new_mon 283002 1 7068 7337 650 0 0 0 2
				new_mon 283003 1 7068 7337 650 0 0 0 2
			endif
			if 10 = 240
				new_mon 283000 7 5668 7337 650 0 0 0 2
				new_mon 283001 1 5668 7337 650 0 0 0 2
				new_mon 283002 1 5668 7337 650 0 0 0 2
				new_mon 283003 1 5668 7337 650 0 0 0 2
			endif
			if 10 = 265
				new_mon 283000 7 5668 5937 650 0 0 0 2
				new_mon 283001 1 5668 5937 650 0 0 0 2
				new_mon 283002 1 5668 5937 650 0 0 0 2
				new_mon 283003 1 5668 5937 650 0 0 0 2
			endif
			if 10 = 290
				new_mon 283000 7 7068 5937 650 0 0 0 2
				new_mon 283001 1 7068 5937 650 0 0 0 2
				new_mon 283002 1 7068 5937 650 0 0 0 2
				new_mon 283003 1 7068 5937 650 0 0 0 2
			endif
			if 10 = 315
				new_mon 283000 4 7068 7337 650 0 0 0 2
				new_mon 283001 2 7068 7337 650 0 0 0 2
				new_mon 283002 2 7068 7337 650 0 0 0 2
				new_mon 283003 2 7068 7337 650 0 0 0 2
			endif
			if 10 = 340
				new_mon 283000 4 5668 7337 650 0 0 0 2
				new_mon 283001 2 5668 7337 650 0 0 0 2
				new_mon 283002 2 5668 7337 650 0 0 0 2
				new_mon 283003 2 5668 7337 650 0 0 0 2
			endif
			if 10 = 365
				new_mon 283000 4 5668 5937 650 0 0 0 2
				new_mon 283001 2 5668 5937 650 0 0 0 2
				new_mon 283002 2 5668 5937 650 0 0 0 2
				new_mon 283003 2 5668 5937 650 0 0 0 2
			endif

			;加保护
			get_pworld_var 20 32 1 14
			get_pworld_var 8 32 1 21
			if 10 > 365 and 14 = 0 and @21 < 474
				var 21 474
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 474 and 17 = 13 and 14 = 0
				var 21 475
				set_pworld_var 8 32 @21 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 474 and 17 = 12
				new_mon 227005 1 6368 6637 0 0 0 0 2
				var 17 13
				set_pworld_var 32 32 @17 1
			endif
			
			get_pworld_var 8 32 1 21
			get_pworld_var 32 32 1 17
			if 21 = 475 and 17 = 13
				var 17 14
				set_pworld_var 32 32 @17 1
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			if 17 = 14 and 10 >= 2

				add_sys_msg_start
				add_sys_msg 祝贺大家通过第七阶段，最终挑战霸王龙将于20秒后到来
				set_sys_msg_mode 4 0 0
				map_msg_info
				add 11 1
				set_pworld_var 4 32 @11 1
				var 10 0
				set_pworld_var 0 32 @10 1
				var 13 0
				set_pworld_var 12 32 @13 1
			endif

		endcase	




		
	;放出霸王龙
		case 9
			get_pworld_var 16 32 1 12	
			;if 21 = 475
				add 12 1


				IF 12 = 5
					add_sys_msg_start
					add_sys_msg 10秒后，将与龙蛋谷的主人-霸王龙进行最后的决战!
					set_sys_msg_mode 4 0 0
					map_msg_info
				endif

				IF 12 = 8
					EFFECT 0 0 playEffct(6268,6060,"effect\\entity\\281001_18.ent")
					new_mon 281001 1 6268 6060 0 0 0 0 50
					TARGET_ADD_STATUS 3 @50 1095 1 7200 2
				endif

				if 12 = 10
					add_sys_msg_start
					add_sys_msg 看着子嗣们的倒下，霸王龙发出了这辈子来最歇斯底里地哀号，也预示着最终挑战的到来。
					set_sys_msg_mode 4 0 0
					map_msg_info
					add 11 1
					set_pworld_var 4 32 @11 1
					var 10 0
					set_pworld_var 0 32 @10 1
				endif
				set_pworld_var 16 32 @12 1
			;ENDIF

		endcase
		;case 9 
			;if 21 = 317
				;get_pworld_var 20 32 1 14
				;add 14 1
				;set_pworld_var 20 32 @14 1
				;get_pworld_var 20 32 1 14
				;if 14 = 3
				;new_npc 1 333003 6568 6460 0 zhu30out.mac
				;pworld_over
				;OPEN_PWORLD_AWARD_UI
				;add_sys_msg_start
				;add_sys_msg 副本已完成，可通过传送门离开
				;set_sys_msg_mode 4 0 0
				;map_msg_info
				;add 11 1
				;set_pworld_var 4 32 @11 1
				;endif
			;endif
		;endcase
		
	endswitch
	
;	判断怪物是否死亡
;	get_mon_num 0 22
;	if	22 = 0
;		if 11 = 9 
;		add 11 1
;		set_pworld_var 4 32 @11 1
;		add_sys_msg_start
;		add_sys_msg 所有怪物已死亡!
;		set_sys_msg_mode 3 0 0
;		map_msg_info
;		endif
;	endif
	
	get_pworld_var 56 8 1 24
	if 24 = 1
	;霸王龙死亡
		get_pworld_var 57 32 1 25
		add 25 1
		set_pworld_var 57 32 @25 1
		if 25 = 1
			add_sys_msg_start
			add_sys_msg 龙蛋谷已通关，120秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 25 = 30
			add_sys_msg_start
			add_sys_msg 龙蛋谷已通关，60秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 25 = 45
			add_sys_msg_start
			add_sys_msg 龙蛋谷已通关，30秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 25 = 55
			add_sys_msg_start
			add_sys_msg 龙蛋谷已通关，10秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 25 = 60
			pworld_over
			COMPEL_CLOSE_PWORLD 1
			return
		endif
	endif
endproc


proc 4
	GET_MON_INFO 0 300 0 0 0 0 1 1
	if @300 = 283000 or @300 = 283001 or @300 = 283002 or @300 = 283003 or @300 = 283010 or @300 = 283011 or @300 = 283012 or @300 = 283013 or @300 = 283004 or @300 = 283005 or @300 = 283006 or @300 = 283007 or @300 = 283008 or @300 = 283009 or @300 = 283019
	get_pworld_var 8 32 1 21
	add 21 1
	set_pworld_var 8 32 @21 1
	
;	If 21 < 216
;		add_sys_msg_start
;		add_sys_msg 杀怪数/总数：$@21$/215
;		set_sys_msg_mode 4 0 0
;		map_msg_info
;	endif

	
	
	
	
	get_pworld_var 12 32 1 13
	get_pworld_var 32 32 1 17
	switch 17
		case 0
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 1/7,怪物击杀 $@13$/10
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 2
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 2/7,怪物击杀 $@13$/20
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 4
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 3/7,怪物击杀 $@13$/40
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 6
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 4/7,怪物击杀 $@13$/60
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 8
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 5/7,怪物击杀 $@13$/80
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 10
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 6/7,怪物击杀 $@13$/108
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 12
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 7/7,怪物击杀 $@13$/150
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase


	endswitch
	endif

	if @300 = 283029 or @300 = 227005
	get_pworld_var 8 32 1 21
	add 21 1
	set_pworld_var 8 32 @21 1


	get_pworld_var 12 32 1 13
	get_pworld_var 32 32 1 17
	switch 17
		case 0
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 1/7,怪物击杀 $@13$/10
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 2
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 2/7,怪物击杀 $@13$/20
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 4
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 3/7,怪物击杀 $@13$/40
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 6
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 4/7,怪物击杀 $@13$/60
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 8
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 5/7,怪物击杀 $@13$/80
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 10
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 6/7,怪物击杀 $@13$/108
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
		case 12
			add 13 1
			set_pworld_var 12 32 @13 1
			add_sys_msg_start
			add_sys_msg 副本进度：阶段 7/7,怪物击杀 $@13$/150
			set_sys_msg_mode 3 0 0
			map_msg_info
		endcase
	endswitch


	endif
endproc

	