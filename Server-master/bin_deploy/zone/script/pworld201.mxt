;夺宝奇兵

proc 1
	add_sys_msg_start
	add_sys_msg 狂欢开始了！
	set_sys_msg_mode 4 0 0
	map_msg_info


	var 10 0
	set_pworld_var 4 32 @10 1 
	;设置计时器变量1
	var 11 0
	set_pworld_var 8 32 @11 1 
	;设置随机数

	var 12 0
	set_pworld_var 12 32 @12 1
	;宝箱数量
	var 13 0
	set_pworld_var 16 32 @13 1
	;道具数量
	var 14 0
	set_pworld_var 20 32 @14 1
	;玩家数量
	var 15 0
	set_pworld_var 24 32 @15 1
	var 16 0
	set_pworld_var 28 32 @16 1
	var 17 0
	set_pworld_var 32 32 @17 1
	var 18 0
	set_pworld_var 36 32 @18 1
	var 19 0
	set_pworld_var 40 32 @19 1
	var 20 0
	set_pworld_var 44 32 @20 1
	var 21 0
	set_pworld_var 48 32 @21 1
	var 22 0
	set_pworld_var 52 32 @22 1
	var 23 0
	set_pworld_var 56 32 @23 1
	var 24 0
	set_pworld_var 60 32 @24 1
	;刷新坐标，15-20内场，21-24外场
	var 25 0
	set_pworld_var 64 32 @25 1
	;随机数
	var 26 0
	set_pworld_var 68 32 @26 1
	;道具id
	var 27 0
	set_pworld_var 72 32 @27 1
	;随机数
	var 28 0
	set_pworld_var 76 32 @28 1
	var 29 0
	set_pworld_var 80 32 @29 1
	var 30 0
	set_pworld_var 84 32 @30 1
	var 31 0
	set_pworld_var 86 32 @31 1
	;副本剩余时间
	

	new_mon 234001 1 7600 9600 0 58 0 0 0
	new_mon 234001 1 9600 7600 0 59 0 0 0
	new_mon 234001 1 11600 9600 0 60 0 0 0
	new_mon 234001 1 9600 11600 0 61 0 0 0
	new_mon 234001 1 8100 9600 0 62 0 0 0
	new_mon 234001 1 9600 8100 0 63 0 0 0
	new_mon 234001 1 11100 9600 0 64 0 0 0
	new_mon 234001 1 9600 11100 0 65 0 0 0
	;new_mon 234001 1 8600 9600 0 66 0 0 0
	;new_mon 234001 1 9600 8600 0 67 0 0 0
	;new_mon 234001 1 10600 9600 0 68 0 0 0
	;new_mon 234001 1 9600 10600 0 69 0 0 0

	var 32 0
	set_pworld_var 90 8 @32 1
	;计时器


endproc


proc 3

	get_pworld_info 0 0 31
	set_pworld_var 86 32 @31 1
	if @31 < 21
		add_sys_msg_start
		add_sys_msg 嘉年华即将关闭，倒计时$@31$秒
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif

	GET_PWORLD_VAR 4 32 1 10
	;得到计时变量
	GET_PWORLD_VAR 8 32 1 11
	add 10 1
	set_pworld_var 4 32 @10 1
	if 10 = 15
		rand 11 6
		add 11 5
		mul 11 20
		; get_id_player_info 0 19 30
		; if 30 != -1
			; var 11 @30
			; dev 11 10000
			
	
			RANGE_ADD_EXP 0 @11 1
			if 1 < 0
				add_sys_msg_start
				add_sys_msg 加经验失败	
				set_sys_msg_mode 4 0 0
				map_msg_info
			endif
		;endif
		var 10 0
		set_pworld_var 4 32 @10 1
	endif
	
	GET_PWORLD_ROLE_NUM 14 0
	set_pworld_var 20 32 @14 1
	var 28 @14
	var 29 @14
	var 30 @14
	mul 28 4
	dev 29 2
	dev 30 10
	set_pworld_var 76 32 @28 1
	set_pworld_var 80 32 @29 1
	set_pworld_var 84 32 @30 1
	GET_MON_NUM_BY_DEFID 293025 12
	set_pworld_var 12 32 @12 1
	while @12 < @28 and @12 < 20
	
		;add_sys_msg_start
		;add_sys_msg 12 = $@12$ ,28 = $@28$
		;set_sys_msg_mode 4 0 0
		;map_msg_info
		
	rand 27 10
		switch 27
			case 0
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 7600 9600 800 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8502 8502 800 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 7600 800 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10698 8502 800 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11600 9600 800 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10698 10698 800 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11600 800 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8502 10698 800 0 0 0 1
					endcase
				endswitch
			endcase
			case 1
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 800 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8868 8868 800 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 800 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10332 8868 800 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 800 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10332 10332 800 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 800 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8868 10332 800 0 0 0 1
					endcase
				endswitch
			endcase
			case 2
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase
			case 3
				rand 25 10
				switch 25
					case 0
					new_mon 293025 1 6835 11982 800 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 6848 10552 800 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 6870 8866 800 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 6848 7380 800 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 8751 12161 800 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10434 12137 800 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 12028 12172 800 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 12086 10414 800 0 0 0 1
					endcase
					case 8
					new_mon 293025 1 12083 8981 800 0 0 0 1
					endcase
					case 9
					new_mon 293025 1 12173 7234 800 0 0 0 1
					endcase
				endswitch
			endcase
			case 4
				rand 25 7
				switch 25
					case 0
					new_mon 293025 1 13891 6324 800 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 14114 4716 800 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 11434 5120 800 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10526 4968 800 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 8944 4929 800 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 7015 5041 800 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9689 6747 800 0 0 0 1
					endcase
				endswitch
			endcase
			case 5
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase
			case 6
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase
			case 8
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase
			case 7
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase
			case 9
				rand 25 8
				switch 25
					case 0
					new_mon 293025 1 8100 9600 500 0 0 0 1
					endcase
					case 1
					new_mon 293025 1 8600 8600 500 0 0 0 1
					endcase
					case 2
					new_mon 293025 1 9600 8100 500 0 0 0 1
					endcase
					case 3
					new_mon 293025 1 10600 8600 500 0 0 0 1
					endcase
					case 4
					new_mon 293025 1 11100 9600 500 0 0 0 1
					endcase
					case 5
					new_mon 293025 1 10600 10600 500 0 0 0 1
					endcase
					case 6
					new_mon 293025 1 9600 11100 500 0 0 0 1
					endcase
					case 7
					new_mon 293025 1 8600 10600 500 0 0 0 1
					endcase
				endswitch
			endcase

		endswitch
		GET_MON_NUM_BY_DEFID 293025 12
		set_pworld_var 12 32 @12 1

	endwhile

		

	get_tmp_num 0 0 13
	set_pworld_var 16 32 @13 1
	if 13 < @29 or 13 < 5
		while 30 >= 0

		
		get_pworld_var 90 8 1 32
		add 32 1
		Set_pworld_var 90 8 @32 1
			if 32 = 5
			;每10s刷一次
				call 200
			
				var 32 0
				set_pworld_var 90 8 @32 1
			
			endif
			
			
			
		
		sub 30 1
		set_pworld_var 84 32 @30 1
		endwhile
	endif
	


		
endproc

proc 200


		rand 27 5
			switch 27
				case 0
				rand 25 8
				switch 25
					case 0
					var 15 7600
					var 16 9600
					endcase
					case 1
					var 15 8502
					var 16 8502
					endcase
					case 2
					var 15 9600
					var 16 7600
					endcase
					case 3
					var 15 10658
					var 16 8502
					endcase
					case 4
					var 15 11600
					var 16 9600
					endcase
					case 5
					var 15 10698
					var 16 10698
					endcase
					case 6
					var 15 9600
					var 16 11600
					endcase
					case 7
					var 15 8502
					var 16 10698
					endcase
					set_pworld_var 24 32 @15 1
					set_pworld_var 28 32 @16 1
				endswitch
			
					rand 25 7
					switch 25
						case 0
						var 26 1400100
						endcase
						case 1
						var 26 1400101
						endcase
						case 2
						var 26 1400102
						endcase
						case 3
						var 26 1400103
						endcase
						case 4
						var 26 1400104
						endcase
						case 5
						var 26 1400105
						endcase
						case 6
						var 26 1440100
						endcase
						set_pworld_var 68 32 @26 1
					endswitch
				ADD_TMP_ITEM @26 1 @15 @16 500 0 0 30 3
				endcase
				case 1
					rand 25 8
					switch 25
						case 0
						var 15 8100
						var 16 9600
						endcase
						case 1
						var 15 8868
						var 16 8868
						endcase
						case 2
						var 15 9600
						var 16 8100
						endcase
						case 3
						var 15 10332
						var 16 8868
						endcase
						case 4
						var 15 11100
						var 16 9600
						endcase
						case 5
						var 15 11100
						var 16 11100
						endcase
						case 6
						var 15 9600
						var 16 11100
						endcase
						case 7
						var 15 8868
						var 16 10332
						endcase
						set_pworld_var 24 32 @15 1
						set_pworld_var 28 32 @16 1
					endswitch
					rand 25 7
					switch 25
						case 0
						var 26 1400100
						endcase
						case 1
						var 26 1400101
						endcase
						case 2
						var 26 1400102
						endcase
						case 3
						var 26 1400103
						endcase
						case 4
						var 26 1400104
						endcase
						case 5
						var 26 1400105
						endcase
						case 6
						var 26 1440100
						endcase
						set_pworld_var 68 32 @26 1
					endswitch
					ADD_TMP_ITEM @26 1 @15 @16 300 0 0 30 3
				endcase
				case 2
					rand 25 8
					switch 25
						case 0
						var 15 8600
						var 16 9600
						endcase
						case 1
						var 15 8900
						var 16 8900
						endcase
						case 2
						var 15 9600
						var 16 8600
						endcase
						case 3
						var 15 10300
						var 16 8900
						endcase
						case 4
						var 15 10600
						var 16 9600
						endcase
						case 5
						var 15 10300
						var 16 10300
						endcase
						case 6
						var 15 9600
						var 16 10600
						endcase
						case 7
						var 15 8900
						var 16 10300
						endcase
						set_pworld_var 24 32 @15 1
						set_pworld_var 28 32 @16 1
						endswitch
					rand 25 7
					switch 25
						case 0
						var 26 1400100
						endcase
						case 1
						var 26 1400101
						endcase
						case 2
						var 26 1400102
						endcase
						case 3
						var 26 1400103
						endcase
						case 4
						var 26 1400104
						endcase
						case 5
						var 26 1400105
						endcase
						case 6
						var 26 1440100
						endcase
						set_pworld_var 68 32 @26 1
					endswitch
					ADD_TMP_ITEM @26 1 @15 @16 100 0 0 30 3
				endcase
				case 3
					rand 25 10
					switch 25
						case 0
						var 15 6835
						var 16 11982
						endcase
						case 1
						var 15 6848
						var 16 10552
						endcase
						case 2
						var 15 6870
						var 16 8866
						endcase
						case 3
						var 15 6848
						var 16 7380
						endcase
						case 4
						var 15 8751
						var 16 12161
						endcase
						case 5
						var 15 10434
						var 16 12137
						endcase
						case 6
						var 15 12028
						var 16 12172
						endcase
						case 7
						var 15 12086
						var 16 10414
						endcase
						case 8
						var 15 12083
						var 16 8981
						endcase
						case 9
						var 15 12173
						var 16 7234
						endcase
						set_pworld_var 24 32 @15 1
						set_pworld_var 28 32 @16 1
					endswitch
					rand 25 7
					switch 25
						case 0
						var 26 1400100
						endcase
						case 1
						var 26 1400101
						endcase
						case 2
						var 26 1400102
						endcase
						case 3
						var 26 1400103
						endcase
						case 4
						var 26 1400104
						endcase
						case 5
						var 26 1400105
						endcase
						case 6
						var 26 1440100
						endcase
						set_pworld_var 68 32 @26 1
					endswitch
					ADD_TMP_ITEM @26 1 @15 @16 500 0 0 30 3
				endcase
				case 4
					rand 25 7
					switch 25
						case 0
						var 15 13891
						var 16 6324
						endcase
						case 1
						var 15 14114
						var 16 4716
						endcase
						case 2
						var 15 11434
						var 16 5120
						endcase
						case 3
						var 15 10526
						var 16 4968
						endcase
						case 4
						var 15 8944
						var 16 4929
						endcase
						case 5
						var 15 7015
						var 16 5041
						endcase
						case 6
						var 15 9689
						var 16 6747
						endcase
						set_pworld_var 24 32 @15 1
						set_pworld_var 28 32 @16 1
					endswitch
					rand 25 7
					switch 25
						case 0
						var 26 1400100
						endcase
						case 1
						var 26 1400101
						endcase
						case 2
						var 26 1400102
						endcase
						case 3
						var 26 1400103
						endcase
						case 4
						var 26 1400104
						endcase
						case 5
						var 26 1400105
						endcase
						case 6
						var 26 1440100
						endcase
						set_pworld_var 68 32 @26 1
					endswitch
					ADD_TMP_ITEM @26 1 @15 @16 800 0 0 30 3
				endcase
			endswitch

endproc
















