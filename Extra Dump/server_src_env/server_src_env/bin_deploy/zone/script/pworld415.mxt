PROC 1					;创建副本事件

	add_sys_msg_start
	add_sys_msg 狂沙角斗场正式开始
	set_sys_msg_mode 4 0 0
	map_msg_info
	
	var 10 0
	set_pworld_var 0 32 @10 1
	var 11 0
	set_pworld_var 4 32 @11 1
	VAR 12 0
	set_pworld_var 8 32 @12 1
	VAR 13 0
	set_pworld_var 12 32 @13 1
	var 14 0
	set_pworld_var 16 32 @14 1
	var 15 0
	set_pworld_var 20 32 @15 1
	var 16 0
	set_pworld_var 24 32 @16 1
	var 17 0
	set_pworld_var 28 32 @17 1
	var 18 0
	set_pworld_var 32 32 @18 1
	var 19 0
	set_pworld_var 36 32 @19 1
	var 20 0
	set_pworld_var 40 32 @20 1
	var 21 0
	set_pworld_var 44 32 @21 1
	var 22 0
	set_pworld_var 48 32 @22 1
	var 23 0
	set_pworld_var 52 32 @23 1
	var 24 0
	set_pworld_var 56 32 @24 1
	var 25 0
	set_pworld_var 60 32 @25 1
	var 26 0
	set_pworld_var 64 32 @26 1
	var 27 0
	set_pworld_var 68 32 @27 1
	var 28 0
	set_pworld_var 72 32 @28 1
	var 29 0
	set_pworld_var 76 32 @29 1
	var 30 0
	set_pworld_var 80 32 @30 1
	var 31 0
	set_pworld_var 84 32 @31 1

	
	;设置副本变量。10刷怪计数，11死亡怪物id,12随机数，13、14死亡怪物坐标,15环次，16、17头目出现坐标，18巡逻id,19怪物实例id,20副本怪物数量,21头目数量,22副本玩家数
	;23刷水晶贼数量，24刷矿石贼数量,25、26随机数，27,28为临时道具坐标，29为临时道具id，30为临时道具数量,31刷宝箱计数
ENDPROC

PROC 3
	get_pworld_var 0 32 1 10
	get_pworld_var 84 32 1 31
	ADD 10 1
	ADD 31 1
	set_pworld_var 84 32 @31 1
	set_pworld_var 0 32 @10 1
	get_pworld_var 0 32 1 10
	get_pworld_var 4 32 1 11
	get_pworld_var 8 32 1 12
	get_pworld_var 20 32 1 15
	get_pworld_var 24 32 1 16
	get_pworld_var 28 32 1 17
	get_pworld_var 32 32 1 18
	get_pworld_var 40 32 1 20
	get_pworld_var 44 32 1 21
	get_pworld_var 84 32 1 31
	;add_sys_msg_start
	;add_sys_msg 10=$@10$,11=$@11$,12=$@12$,13=$@13$,14=$@14$,15=$@15$,18=$@18$,20=$@20$,21=$@21$,31=$@31$
	;set_sys_msg_mode 0 0 0
	;map_msg_info
	get_pworld_role_num 22 0
	if 22 < 11
		var 23 1
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	if 22 > 10 and 22 < 21
		var 23 2
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	if 22 > 20 and 22 < 31
		var 23 3
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	if 22 > 30 and 22 < 41
		var 23 4
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	if 22 > 40 and 22 < 51
		var 23 5
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	if 22 > 50 and 22 < 61
		var 23 6
		var 24 1
		set_pworld_var 52 32 @23 1
		set_pworld_var 56 32 @24 1
	endif
	

	if 31 = 5
		get_tmp_num 0 0 30
		if 30 < 20 and 30 < @22
			rand 25 21
			switch 25
				case 0
					var 27 9638
					var 28 7051
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 1
					var 27 8823
						var 28 7558
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 2
					var 27 8048
					var 28 8219
				set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 3
					var 27 7340
					var 28 9353
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 4
					var 27 7020
					var 28 10470
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 5
					var 27 6904
					var 28 11343
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 6
					var 27 6977
					var 28 12323
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 7
					var 27 8315
					var 28 14504
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 8
					var 27 9047
					var 28 15113
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 9
					var 27 9840
					var 28 15448
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 10
					var 27 11138
					var 28 15598
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 11
					var 27 12251
					var 28 15469
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 12
					var 27 13273
					var 28 15078
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 13
					var 27 14170
					var 28 14507
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 14
					var 27 15588
					var 28 12345
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 15
					var 27 15694
					var 28 11265
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 16
					var 27 15566
					var 28 10065
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 17
					var 27 15198
					var 28 9132
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 18
					var 27 14584
					var 28 8317
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 19
					var 27 13660
					var 28 7521
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
				case 20
					var 27 12725
					var 28 7105
					set_pworld_var 68 32 @27 1
					set_pworld_var 72 32 @28 1
				endcase
			endswitch
			rand 22 8
			switch 22
				case 0
					var 29 1440001
					set_pworld_var 76 32 @29 1
				endcase
				case 1
					var 29 1400106
					set_pworld_var 76 32 @29 1
				endcase
				case 2
					var 29 1400107
					set_pworld_var 76 32 @29 1
				endcase
				case 3
					var 29 1400108
					set_pworld_var 76 32 @29 1
				endcase
				case 4
					var 29 1400109
					set_pworld_var 76 32 @29 1
				endcase
				case 5
					var 29 1440101
					set_pworld_var 76 32 @29 1
				endcase
				case 6
					var 29 1400110
					set_pworld_var 76 32 @29 1
				endcase
				case 7
					var 29 1400111
					set_pworld_var 76 32 @29 1
				endcase
			endswitch
			add_tmp_item @29 1 @27 @28 300 0 0 0 2
			;new_tmp_item @29 @27 @28 0 0 60 0
			;add_sys_msg_start
			;add_sys_msg 临时道具刷新了
			;set_sys_msg_mode 0 0 0
			;map_msg_info
		else
			;add_sys_msg_start
			;add_sys_msg 临时道具太多了
			;set_sys_msg_mode 0 0 0
			;map_msg_info	
		endif
		
		var 31 0
		set_pworld_var 84 32 @31 1
	endif



	while 15 > 5
		sub 15 6
		set_pworld_var 20 32 @15 1
	endwhile
	
	switch 15
		case 0
			var 16 9459
			var 17 12265
			var 18 47
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase


		case 1
			var 16 9179
			var 17 10196
			var 18 48
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase

		case 2
			var 16 11245
			var 17 9252
			var 18 49
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase

		case 3
			var 16 13247
			var 17 10270
			var 18 50
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase

		case 4
			var 16 12921
			var 17 12376
			var 18 51
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase

		case 5
			var 16 11057
			var 17 13606
			var 18 52
			set_pworld_var 24 32 @16 1
			set_pworld_var 28 32 @17 1
			set_pworld_var 32 32 @18 1
		endcase
	ENDSWITCH
	
	GET_MON_NUM 0 20
	set_pworld_var 40 32 @20 1
	if 20 > 50 or 20 > @22
		var 10 0
		set_pworld_var 0 32 @10 1
	else
		switch 10
			case 1
			new_dyn_mon 2 283016 @23 11344 12927 800 0 0 0 0 90 1
			endcase
			case 3
			new_dyn_mon 2 283016 @23 9392 10392 800 0 0 0 0 90 1
			endcase
			case 5
			new_dyn_mon 2 283016 @23 12778 10605 800 0 0 0 0 90 1
			endcase
			case 7
			new_dyn_mon 2 283015 @23 11337 12069 800 0 0 0 0 90 1
			endcase
			case 9
			new_dyn_mon 2 283015 @23 10690 11027 800 0 0 0 0 90 1
			endcase
			case 10
			new_dyn_mon 2 283015 @23 11914 10976 800 0 0 0 0 90 1
			endcase
			case 15
			GET_MON_NUM_BY_DEFID 283017 21
			set_pworld_var 44 32 @21 1
			if 21 = 0
				add_sys_msg_start
				add_sys_msg 窃贼头目要逃跑了
				set_sys_msg_mode 4 0 0
				map_msg_info
				new_dyn_mon 19 283017 1 @16 @17 0 @18 0 0 0 90 1
				set_pworld_var 36 32 @19 1
				get_pworld_var 36 32 1 19
				TARGET_ADD_STATUS 3 @19 1095 1 7200 2
				var 10 0
				add 15 1
				set_pworld_var 20 32 @15 1
				set_pworld_var 0 32 @10 1
			else
				var 10 0
				set_pworld_var 0 32 @10 1
			endif
			endcase
		endswitch
	endif


ENDPROC

PROC 4
	GET_MON_INFO 0 11 0 0 0 0 13 14
	set_pworld_var 4 32 @11 1

	if 11 = 283015
		RAND 12 30
		if 12 = 0
			NEW_MON 293071 1 @13 @14 0 0 0 1 2
		endif
		ADD_PWORLD_RESOURCE 0 0 1 2
	endif
	;击杀水晶窃贼获得1水晶
	
	if 11 = 283016
		RAND 12 30
		if 12 = 0
			NEW_MON 293071 1 @13 @14 0 0 0 1 2
		endif
		ADD_PWORLD_RESOURCE 0 1 1 2
	endif
	;击杀矿石窃贼获得1矿石

	if 11 = 283017
		RAND 12 30
		if 12 = 0
			NEW_MON 293071 1 @13 @14 0 0 0 1 2
		endif
		ADD_PWORLD_RESOURCE 0 0 10 2
	endif
	;击杀窃贼头目获得10水晶

	if 11 = 283014
		add_sys_msg_start
		add_sys_msg 角斗士之魂怨念耗尽，消散在空气中
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
	;角斗士之魂消失时提示

ENDPROC

proc 5
		add_sys_msg_start
		add_sys_msg 击杀窃贼可获得“水晶”或“矿石”资源，击败其他玩家将获得该玩家10%的资源。
		set_sys_msg_mode 7 0 0
		MSG_INFO
endproc