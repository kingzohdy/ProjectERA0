;盘山道

proc 1

	add_sys_msg_start
	add_sys_msg 山道中分布着四根独特的石柱，解开封印的钥石就藏在其中
	set_sys_msg_mode 4 0 0
	map_msg_info

	
;炮台
	new_tmp_item 1460002 6085 8242 90 0 0 1
	;NEW_TMP_ITEM 1400002 7680 10616 0 0 0  1
	new_mon 290002 1 3394 7079 0 0 0 0 1

	var 10 0
	set_pworld_var 4 32 @10 1
	;第一个封印
	var 13 0
	set_pworld_var 8 32 @13 1
	;第二个封印
	var 14 0
	set_pworld_var 12 32 @14 1
	;第三个封印
	var 15 0
	set_pworld_var 16 32 @15 1
	;第四个封印
	var 16 0
	set_pworld_var 20 32 @16 1
	;第一个狂暴者
	var 17 0
	set_pworld_var 24 32 @17 1
	;第二个狂暴者
	var 18 0
	set_pworld_var 28 32 @18 1
	;第三个狂暴者
	var 19 0
	set_pworld_var 32 32 @19 1
	;第四个狂暴者
	var 20 0
	set_pworld_var 36 32 @20 1
	;第五个狂暴者
	var 21 0
	set_pworld_var 40 32 @21 1
	;第六个狂暴者
	var 22 0
	set_pworld_var 44 32 @22 1
	;第七个狂暴者
	var 23 0
	set_pworld_var 48 32 @23 1
	;第八个狂暴者
	var 24 0
	set_pworld_var 52 32 @24 1
	;第九个狂暴者
	var 25 0
	set_pworld_var 56 32 @25 1
	;第十个狂暴者
	var 26 0
	set_pworld_var 60 32 @26 1
	;第十一个狂暴者
	var 27 0
	set_pworld_var 64 32 @27 1
	;第十二个狂暴者
	var 28 0
	set_pworld_var 68 32 @28 1
	;第十三个狂暴者
	var 29 0
	set_pworld_var 72 32 @29 1
	;第十四个狂暴者
	var 30 0
	set_pworld_var 76 32 @30 1
	;第十五个狂暴者
	var 31 0
	set_pworld_var 80 32 @31 1
	;第十六个狂暴者
	var 32 0
	set_pworld_var 88 32 @32 1
	;阶段标志
	var 33 0
	set_pworld_var 92 32 @33 1
	;计时器


	get_map_mon_id 284000 34 1
	set_pworld_var 96 32 @34 1
	TARGET_ADD_STATUS 3 @34 1095 1 7200 2
	TARGET_ADD_STATUS 3 @34 1033 2 7200 2
	TARGET_ADD_STATUS 3 @34 1033 3 7200 2
	TARGET_ADD_STATUS 3 @34 1033 4 7200 2
	TARGET_ADD_STATUS 3 @34 1033 5 7200 2
	;第一个石柱
	get_map_mon_id 284001 35 1
	set_pworld_var 100 32 @35 1
	TARGET_ADD_STATUS 3 @35 1095 1 7200 2
	TARGET_ADD_STATUS 3 @35 1033 2 7200 2
	TARGET_ADD_STATUS 3 @35 1033 3 7200 2
	TARGET_ADD_STATUS 3 @35 1033 4 7200 2
	TARGET_ADD_STATUS 3 @35 1033 5 7200 2
	;第二个石柱
	get_map_mon_id 284002 36 1
	set_pworld_var 104 32 @36 1
	TARGET_ADD_STATUS 3 @36 1095 1 7200 2
	TARGET_ADD_STATUS 3 @36 1033 2 7200 2
	TARGET_ADD_STATUS 3 @36 1033 3 7200 2
	TARGET_ADD_STATUS 3 @36 1033 4 7200 2
	TARGET_ADD_STATUS 3 @36 1033 5 7200 2
	;第三个石柱
	get_map_mon_id 284003 37 1
	set_pworld_var 108 32 @37 1
	TARGET_ADD_STATUS 3 @37 1095 1 7200 2
	TARGET_ADD_STATUS 3 @37 1033 2 7200 2
	TARGET_ADD_STATUS 3 @37 1033 3 7200 2
	TARGET_ADD_STATUS 3 @37 1033 4 7200 2
	TARGET_ADD_STATUS 3 @37 1033 5 7200 2
	;第四个石柱



	;new_mon 200003 1 10896 8710 0 0 0 0 16
	;set_pworld_var 20 32 @16 1
	new_mon 200003 1 10896 7510 0 0 0 0 17
	set_pworld_var 24 32 @17 1
	;new_mon 200003 1 9696 8710 0 0 0 0 18
	;set_pworld_var 28 32 @18 1
	;new_mon 200003 1 9696 7510 0 0 0 0 19
	;set_pworld_var 32 32 @19 1
	;new_mon 200003 1 9350 11251 0 0 0 0 20
	;set_pworld_var 36 32 @20 1
	new_mon 200003 1 9350 10051 0 0 0 0 21
	set_pworld_var 40 32 @21 1
	;new_mon 200003 1 8150 11251 0 0 0 0 22
	;set_pworld_var 44 32 @22 1
	;new_mon 200003 1 8150 10051 0 0 0 0 23
	;set_pworld_var 48 32 @23 1
	;new_mon 200003 1 5439 11382 0 0 0 0 24
	;set_pworld_var 52 32 @24 1
	new_mon 200003 1 5439 10182 0 0 0 0 25
	set_pworld_var 56 32 @25 1
	;new_mon 200003 1 4239 11382 0 0 0 0 26
	;set_pworld_var 60 32 @26 1
	;new_mon 200003 1 4239 10182 0 0 0 0 27
	;set_pworld_var 64 32 @27 1
	new_mon 200003 1 3077 8193 0 0 0 0 28
	set_pworld_var 68 32 @28 1
	;new_mon 200003 1 3077 6993 0 0 0 0 29
	;set_pworld_var 72 32 @29 1
	;new_mon 200003 1 1877 8193 0 0 0 0 30
	;set_pworld_var 76 32 @30 1
	;new_mon 200003 1 1877 6993 0 0 0 0 31
	;set_pworld_var 80 32 @31 1

	
	;新增
	var 40 0
	set_pworld_var 112 32 @40 1
	
endproc

proc 3

	get_pworld_var 4 32 1 10
	get_pworld_var 8 32 1 13
	get_pworld_var 12 32 1 14
	get_pworld_var 16 32 1 15
	get_pworld_var 88 32 1 32
	get_pworld_var 92 32 1 33

	if @10 = 1 And @13 = 1 And @14 = 1 And @15 = 1 and @32 = 4
		add 33 1
		set_pworld_var 92 32 @33 1
		;if @33 = 1
		;add_sys_msg_start
		;add_sys_msg 酋长苏醒：封印解除 4/4
		;set_sys_msg_mode 3 0 0
		;map_msg_info
		;endif
		if @33 = 5
		add_sys_msg_start
		add_sys_msg 目标完成，野人酋长将在30秒后苏醒。。。。。
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
		if @33 = 20
		;删除兽人酋长npc放出怪物兽人酋长
		del_npc 315001 1
		new_mon_by_dir 282002 1 4628 2218 5 0 0 45 0 100
		TARGET_ADD_STATUS 3 @100 1095 1 7200 2
		var 32 5
		set_pworld_var 88 32 @32 1
		endif
	endif

	;判断兽人酋长是否已死
	
	if @32 = 6

	;	pworld_over
		
		get_pworld_var 112 32 1 40 
		add 40 1
		set_pworld_var 112 32 @40 1
		if 40 = 1
			new_npc 1 333017 5579 1815 0 zhu15out2.mac			
			add_sys_msg_start
			add_sys_msg 副本已完成，可通过传送门离开
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		if 40 = 2
		
			add_sys_msg_start
			add_sys_msg 盘山道已通关，120秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 40 = 30
			add_sys_msg_start
			add_sys_msg 盘山道已通关，60秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 40 = 45
			add_sys_msg_start
			add_sys_msg 盘山道已通关，30秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 40 = 55
			add_sys_msg_start
			add_sys_msg 盘山道已通关，10秒后将自动关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		
		if 40 = 60
			pworld_over
			COMPEL_CLOSE_PWORLD 1
			return
		endif
		; var 32 3
		; set_pworld_var 88 32 @32 1
	
	endif
	

endproc
proc 4
	get_mon_info 1 301 300 0 0 0 0 0
	get_pworld_var 20 32 1 16
	get_pworld_var 24 32 1 17
	get_pworld_var 28 32 1 18
	get_pworld_var 32 32 1 19
	get_pworld_var 36 32 1 20
	get_pworld_var 40 32 1 21
	get_pworld_var 44 32 1 22
	get_pworld_var 48 32 1 23
	get_pworld_var 52 32 1 24
	get_pworld_var 56 32 1 25
	get_pworld_var 60 32 1 26
	get_pworld_var 64 32 1 27
	get_pworld_var 68 32 1 28
	get_pworld_var 72 32 1 29
	get_pworld_var 76 32 1 30
	get_pworld_var 80 32 1 31
	get_pworld_var 96 32 1 34
	get_pworld_var 100 32 1 35
	get_pworld_var 104 32 1 36
	get_pworld_var 108 32 1 37


	if @301 = 200003
		;add_sys_msg_start
		;add_sys_msg 300=$@300$,34=$@34$,35=$@35$,36=$@36$,37=$@37$
		;set_sys_msg_mode 3 0 0
		;map_msg_info

	if @300 = @16
		new_mon 200003 1 9696 8710 0 0 0 0 18
		set_pworld_var 28 32 @18 1
		new_mon 200000 1 9796 8610 0 0 0 0 1
		new_mon 200000 1 9596 8810 0 0 0 0 1
		new_mon 200001 1 9796 8810 0 0 0 0 1
		new_mon 200001 1 9596 8610 0 0 0 0 1
	
		TARGET_ADD_STATUS 3 @34 1033 2 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif

	if @300 = @17
		new_mon 200003 1 10896 8710 0 0 0 0 16
		set_pworld_var 20 32 @16 1
		new_mon 200000 1 10996 8610 0 0 0 0 1
		new_mon 200000 1 10796 8810 0 0 0 0 1
		new_mon 200001 1 10996 8810 0 0 0 0 1
		new_mon 200001 1 10796 8610 0 0 0 0 1

		TARGET_ADD_STATUS 3 @34 1033 3 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @18
		new_mon 200003 1 9696 7510 0 0 0 0 19
		set_pworld_var 32 32 @19 1
		new_mon 200000 1 9796 7410 0 0 0 0 1
		new_mon 200000 1 9596 7610 0 0 0 0 1
		new_mon 200001 1 9796 7610 0 0 0 0 1
		new_mon 200001 1 9596 7410 0 0 0 0 1

		TARGET_ADD_STATUS 3 @34 1033 4 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif

	if @300 = @19
		TARGET_ADD_STATUS 3 @34 1033 5 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif

	if @300 = @20
		
		new_mon 200003 1 8150 11251 0 0 0 0 22
		set_pworld_var 44 32 @22 1
		new_mon 200000 1 8250 11151 0 0 0 0 1
		new_mon 200000 1 8050 11351 0 0 0 0 1
		new_mon 200001 1 8250 11351 0 0 0 0 1
		new_mon 200001 1 8050 11151 0 0 0 0 1

		TARGET_ADD_STATUS 3 @35 1033 2 0 2

		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @21
		new_mon 200003 1 9350 11251 0 0 0 0 20
		set_pworld_var 36 32 @20 1
		new_mon 200000 1 9450 11151 0 0 0 0 1
		new_mon 200000 1 9250 11351 0 0 0 0 1
		new_mon 200001 1 9450 11351 0 0 0 0 1
		new_mon 200001 1 9250 11151 0 0 0 0 1

		TARGET_ADD_STATUS 3 @35 1033 3 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @22
	
		new_mon 200003 1 8150 10051 0 0 0 0 23
		set_pworld_var 48 32 @23 1
		new_mon 200000 1 8250 9951 0 0 0 0 1
		new_mon 200000 1 8050 10151 0 0 0 0 1
		new_mon 200001 1 8250 10151 0 0 0 0 1
		new_mon 200001 1 8050 9951 0 0 0 0 1

		TARGET_ADD_STATUS 3 @35 1033 4 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif

	endif
	if @300 = @23

		TARGET_ADD_STATUS 3 @35 1033 5 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @24
		
		new_mon 200003 1 4239 11382 0 0 0 0 26
		set_pworld_var 60 32 @26 1
		new_mon 200000 1 4339 11282 0 0 0 0 1
		new_mon 200000 1 4139 11482 0 0 0 0 1
		new_mon 200001 1 4339 11482 0 0 0 0 1
		new_mon 200001 1 4139 11282 0 0 0 0 1

		TARGET_ADD_STATUS 3 @36 1033 2 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @25
		
		new_mon 200003 1 5439 11382 0 0 0 0 24
		set_pworld_var 52 32 @24 1
		new_mon 200000 1 5539 11282 0 0 0 0 1
		new_mon 200000 1 5339 11482 0 0 0 0 1
		new_mon 200001 1 5539 11482 0 0 0 0 1
		new_mon 200001 1 5339 11282 0 0 0 0 1

		TARGET_ADD_STATUS 3 @36 1033 3 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @26
		
		new_mon 200003 1 4239 10182 0 0 0 0 27
		set_pworld_var 64 32 @27 1
		new_mon 200000 1 4339 10082 0 0 0 0 1
		new_mon 200000 1 4139 10282 0 0 0 0 1
		new_mon 200001 1 4339 10282 0 0 0 0 1
		new_mon 200001 1 4139 10082 0 0 0 0 1

		TARGET_ADD_STATUS 3 @36 1033 4 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @27
		TARGET_ADD_STATUS 3 @36 1033 5 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @28
		new_mon 200003 1 1877 8193 0 0 0 0 30
		set_pworld_var 76 32 @30 1
		new_mon 200000 1 1977 8093 0 0 0 0 1
		new_mon 200000 1 1777 8293 0 0 0 0 1
		new_mon 200001 1 1977 8293 0 0 0 0 1
		new_mon 200001 1 1777 8093 0 0 0 0 1

		TARGET_ADD_STATUS 3 @37 1033 2 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @29

		TARGET_ADD_STATUS 3 @37 1033 3 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @30
		new_mon 200003 1 1877 6993 0 0 0 0 31
		set_pworld_var 80 32 @31 1
		new_mon 200000 1 1977 6893 0 0 0 0 1
		new_mon 200000 1 1777 7093 0 0 0 0 1
		new_mon 200001 1 1977 7093 0 0 0 0 1
		new_mon 200001 1 1777 6893 0 0 0 0 1

		TARGET_ADD_STATUS 3 @37 1033 4 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif
	if @300 = @31
		new_mon 200003 1 3077 6993 0 0 0 0 29
		set_pworld_var 72 32 @29 1
		new_mon 200000 1 3177 6893 0 0 0 0 1
		new_mon 200000 1 2977 7093 0 0 0 0 1
		new_mon 200001 1 3177 7093 0 0 0 0 1
		new_mon 200001 1 2977 6893 0 0 0 0 1

		TARGET_ADD_STATUS 3 @37 1033 5 0 2
		if @2 = 0
		add_sys_msg_start
		add_sys_msg 附近的封印石解除了一层防护状态
		set_sys_msg_mode 4 0 0
		map_msg_info
		endif
	endif

	endif


	
endproc

proc 5
	effect 2 0 playEffct(10833,6359,"effect\\entity\\9598_16.ent")
endproc