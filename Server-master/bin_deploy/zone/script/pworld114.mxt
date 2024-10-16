;70副本,70级副本
;创建事件
PROC 1	
	;设置副本变量
	call 100
	;各种怪
	call 98
	;各种阻挡
	call 701
	add_sys_msg_start
	add_sys_msg 副本开始
	set_sys_msg_mode 4 0 0
	map_msg_info	
ENDPROC
;关闭事件
proc 2
	add_sys_msg_start
	add_sys_msg 副本结束
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc
;2秒一次
PROC 3
	;副本阶段,主要用于tick时要触发的proc
	get_pworld_var 12 32 1 13
	;开始猜密码
	if 13 = 1
		add 13 1
		set_pworld_var 12 32 @13 1
		;10计时数
		var 10 3
		sub 10 1
		set_pworld_var 0 32 @10 1
	endif
	;密码猜测中
	if 13 = 2
		;密码光环站对次数
		VAR 14 0
		get_pworld_var 16 32 1 14
		;站对6次
		if 14 >= 6
			
			
			
			;10计时数
			get_pworld_var 0 32 1 10
			add 10 1
			;蓝光环死亡后n个tick
			if 10 = 3
				;清空计时
				var 10 0
				set_pworld_var 0 32 @10 1
				add 13 1
				set_pworld_var 12 32 @13 1
				;密码过关
				call 203
			endif
			set_pworld_var 0 32 @10 1
		else
			;10计时数
			get_pworld_var 0 32 1 10
			add 10 1
			;蓝光环死亡后n个tick
			if 10 = 3
				call 202
			endif
			set_pworld_var 0 32 @10 1
			strvar num
			var num @14
			if 10 = 1 and num > 0
				add_sys_msg_start
				add_sys_msg 第$@num$组密码破译成功！下一组即将开始！
				set_sys_msg_mode 3 0 0
				map_msg_info
			endif
		endif
	endif
	;逻辑区域1标记,0未触发过,1触发过
        GET_PWORLD_VAR 28 32 1 17
        If 17 = 1		
                var 17 2
                set_pworld_var 28 32 @17 1
		call 402
        endif
	;逻辑区域2标记,0未触发过,1触发过
        GET_PWORLD_VAR 32 32 1 18
        If 18 = 1
                var 18 2
                set_pworld_var 32 32 @18 1
		call 403
        endif
	;逻辑区域3标记,0未触发过,1触发过
        GET_PWORLD_VAR 36 32 1 19
        If 19 = 1
                var 19 2
                set_pworld_var 36 32 @19 1
		call 404
        endif
	;boss3对话标记,0未触发过,1触发过
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 1
                var 20 2
                set_pworld_var 40 32 @20 1
		call 405
        endif
	if 20 = 2
		;boss3flag34,boss3npc对话后计时
		var 34 0
		get_pworld_var 96 32 1 34
		add 34 1
		;boss3flag34,boss3npc对话后计时
		
		set_pworld_var 96 32 @34 1
		if 34 > 7
			call 411
			var 20 3
			set_pworld_var 40 32 @20 1
		endif
	endif
	if 20 = 3
		get_mon_num_by_defid 206270 1
		if 1 < 1
			var 20 4
			set_pworld_var 40 32 @20 1
			call 406
		endif
	endif
	;boss3flagclose38,boss3脱离战斗后开门
	var 38 0
	get_pworld_var 112 32 1 38
	if 38 = 1
		call 412
		var 38 2
		;boss3flagclose38,boss3脱离战斗后开门
		
		set_pworld_var 112 32 @38 1
	endif
	;逻辑区域4标记,0未触发过,1触发过
        GET_PWORLD_VAR 68 32 1 27
        If 27 = 1
		var 27 2
                set_pworld_var 68 32 @27 1
		call 408
	endif
	;逻辑区域5标记,0未触发过,1触发过
        GET_PWORLD_VAR 72 32 1 28
        If 28 = 1
		var 28 2
		set_pworld_var 72 32 @28 1
		call 409
	endif
	;逻辑区域6标记,0未触发过,1触发过
        GET_PWORLD_VAR 76 32 1 29
        If 29 = 1
	        var 29 2
                set_pworld_var 76 32 @29 1
		call 410
	endif
	;隐藏关npc对话标记,0未触发过,1触发过可以通过
	get_pworld_var 44 32 1 21
	if 21 = 1
		call 600
		var 21 2
		set_pworld_var 44 32 @21 1
	endif	
	;逻辑区域boss1触发区,0未触发过,1第一排报警,2第一排报警,3第三排报警
	get_pworld_var 80 32 1 30
	if 30 > 0
		;11招boss警报激活后的计时
		get_pworld_var 4 32 1 11
		add 11 1
		set_pworld_var 4 32 @11 1
		mod 11 10
		if 11 = 0
			add 30 1
			if 30 > 3
				var 30 1
			endif
			set_pworld_var 80 32 @30 1  
		endif
	endif
	;boss1flag32,boss1召唤标记,0未触发,1以后为计时
	var 32 0
	get_pworld_var 88 32 1 32
	if 32 = 1
		strvar ret num
		var ret 0
		GET_MON_NUM_BY_DEFID 206297 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206298 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206299 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206300 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206301 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206302 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206303 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206304 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206305 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206306 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206307 num
		add ret @num
		GET_MON_NUM_BY_DEFID 206308 num
		add ret @num
		
;		add_sys_msg_start
;		add_sys_msg num$@ret$
;		set_sys_msg_mode 3 0 0
;		map_msg_info
		if ret < 1
			add 32 1
			;boss1flag32,boss1召唤标记,0未触发,1以后为计时
			
			set_pworld_var 88 32 @32 1
		endif
	endif
	if 32 = 2
		add_sys_msg_start
		add_sys_msg 一个凄厉的声音传来：无双，踩扁这些不知死活的家伙！
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
	if 32 > 1
;		sub 32 1
		if 32 > 20
			;招boss
			call 103
			var 32 0
			;boss1flag32,boss1召唤标记,0未触发,1以后为计时
			
			set_pworld_var 88 32 @32 1
		else
			add 32 1
			;boss1flag32,boss1召唤标记,0未触发,1以后为计时
			
			set_pworld_var 88 32 @32 1
		endif
	endif
	;boss5flag35,boss5战斗后关门
	var 35 0
	get_pworld_var 100 32 1 35
	if 35 = 1
		call 503
		;boss5flag35,boss5战斗后关门
		var 35 2
		set_pworld_var 100 32 @35 1
	endif
	;boss5flag36,boss5脱离战斗后开门
	var 36 0
	get_pworld_var 104 32 1 36
	if 36 > 0
		call 504
		;boss5flag36,boss5脱离战斗后开门
		var 36 0
		set_pworld_var 104 32 @36 1
	endif
	;boss3stage42,boss3战斗阶段
	var 42 0
	get_pworld_var 128 32 1 42
	if 42 = 1
		GET_PLAYER_NUM_RANGE 10 34553 29488 2800
		if 10 < 1
			var 42 2
			;boss3stage42,boss3战斗阶段
			
			set_pworld_var 128 32 @42 1
			;boss3flagclose38,boss3脱离战斗后开门
			var 38 0
			get_pworld_var 112 32 1 38
			if 38 = 0 
				var 38 1
				;boss3flagclose38,boss3脱离战斗后开门
				
				set_pworld_var 112 32 @38 1
			;	add_sys_msg_start
			;	add_sys_msg 123
			;	set_sys_msg_mode 3 0 0
			;	map_msg_info
			endif
			
			
			
		endif
	endif
	
	;测试用的tick
;	call 99
ENDPROC
PROC 4
	strvar x
	strvar y
	strvar id
	GET_MON_INFO 0 id 0 0 0 0 x y
	
	;警报器挂了,判断是否是最里面的
	If id = 206268
;		get_pworld_var 4 32 1 11
;		get_pworld_var 8 32 1 12
;		if x = ::x8:: and 12 = ::y8::
			;boss1出场
;			new_mon 206272 1 19344 10978 0 0 0 1 0
;		endif
	endif
	;第一层boss挂了,清怪
	If id = 206272
		call 102
	endif
	;光能网控制器
	if id = 206246
		delete_mon_by_defid 206245 1
	endif
	if id = 206251
		delete_mon_by_defid 206250 1
	endif
	if id = 206253
		delete_mon_by_defid 206252 1
	endif
	if id = 206255
		delete_mon_by_defid 206254 1
	endif
	if id = 206257
		delete_mon_by_defid 206256 1
	endif
	if id = 206311
		delete_mon_by_defid 206309 1
		;清通道
		call 302
	endif
	
	;第三层boss挂了
	if id = 206274
		call 303
	endif
	;第四层boss挂了
	if id = 206276
		call 407
	endif
	;第五层双面挂了
	if id = 206280
		call 502
	endif
	;第六层人形时挂了
	if id = 206281
		strvar x y
		GET_MON_INFO 1 0 0 0 0 0 x y
		var 10 0
		new_mon_by_dir 206283 1 @x @y 0 0 0 @10 1 2	
	endif
ENDPROC
proc 98
	;第一层
	call 101 
	;测试tick用的,赋值为0关闭此功能
	get_pworld_var 20 32 1 15
	if 15 > 0
		return 
	endif
	;第二层,中枢
	call 201
	;第三层,左边boss,及通道
	call 301
	;第四层,右边boss,及通道
	call 401
	;第五层,心脏boss,及通道
	call 501
	;第六层,隐藏boss
	call 601
endproc
proc 99
	;测试tick用的,赋值为0关闭此功能
	get_pworld_var 20 32 1 15
	if 15 < 1
		return 
	endif
	;测试tick用的,阶段
	get_pworld_var 24 32 1 16
	if 16 = 0
		return
	elseif 16 = 1
		call 201
	elseif 16 = 2
		call 301
	elseif 16 = 3
		call 401
	elseif 16 = 4
		call 501
	elseif 16 = 5
		call 601
	else
	endif
	add_sys_msg_start
	add_sys_msg 进入测试阶段$@16$
	set_sys_msg_mode 4 0 0
	map_msg_info
	var 16 0
	set_pworld_var 24 32 @16 1
endproc
proc 100
	;设置副本变量。
	;10计时数
	var 10 0
	set_pworld_var 0 32 @10 1
	;11招boss警报激活后的计时
	var 11 0
	set_pworld_var 4 32 @11 1
	;12招boss警报怪的位置y
;	VAR 12 11026
;	set_pworld_var 8 32 @12 1
	;副本阶段,主要用于tick时要触发的proc
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;密码光环站对次数
	VAR 14 0
	set_pworld_var 16 32 @14 1
	;测试tick用的,赋值为0关闭此功能
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;测试tick用的,阶段
	VAR 16 0
	set_pworld_var 24 32 @16 1
	;逻辑区域1标记,0未触发过,1触发过
	VAR 17 0
	set_pworld_var 28 32 @17 1
	;逻辑区域2标记,0未触发过,1触发过
	VAR 18 0
	set_pworld_var 32 32 @18 1
	;逻辑区域3标记,0未触发过,1触发过
	VAR 19 0
	set_pworld_var 36 32 @19 1
	;boss3对话标记,0未触发过,1触发过
	VAR 20 0
	set_pworld_var 40 32 @20 1
	;隐藏关npc对话标记,0未触发过,1触发过可以通过
	VAR 21 0
	set_pworld_var 44 32 @21 1
	;逻辑区域road1标记,0未触发过,1触发过
	VAR 22 0
	set_pworld_var 48 32 @22 1
	;逻辑区域road2标记,0未触发过,1触发过
	VAR 23 0
	set_pworld_var 52 32 @23 1
	;逻辑区域road3标记,0未触发过,1触发过
	VAR 24 0
	set_pworld_var 56 32 @24 1
	;逻辑区域road4标记,0未触发过,1触发过
	VAR 25 0
	set_pworld_var 60 32 @25 1
	;逻辑区域road5标记,0未触发过,1触发过
	VAR 26 0
	set_pworld_var 64 32 @26 1
	;逻辑区域4标记,0未触发过,1触发过
	VAR 27 0
	set_pworld_var 68 32 @27 1
	;逻辑区域5标记,0未触发过,1触发过
	VAR 28 0
	set_pworld_var 72 32 @28 1
	;逻辑区域6标记,0未触发过,1触发过
	VAR 29 0
	set_pworld_var 76 32 @29 1
	;逻辑区域boss1触发区,0未触发过,1第一排报警,2第一排报警,3第三排报警
	VAR 30 0
	set_pworld_var 80 32 @30 1
	;roadtipflag31,走廊提示逻辑区标记,0未触发过,1触发
	var 31 0
	set_pworld_var 84 32 @31 1
	;boss1flag32,boss1召唤标记,0未触发,1以后为计时
	var 32 0
	set_pworld_var 88 32 @32 1
	
	;ufoflag33,ufo重生标志,0重生,1不重生
	var 33 0
	set_pworld_var 92 32 @33 1
	
	;boss3flag34,boss3npc对话后计时
	var 34 0
	set_pworld_var 96 32 @34 1
	;boss5flag35,boss5战斗后关门
	var 35 0
	set_pworld_var 100 32 @35 1
	;boss5flag36,boss5脱离战斗后开门
	var 36 0
	set_pworld_var 104 32 @36 1
	;boss3flagopen37,boss3战斗后关门
	var 37 0
	set_pworld_var 108 32 @37 1
	;boss3flagclose38,boss3脱离战斗后开门
	var 38 0
	set_pworld_var 112 32 @38 1
	;boss6flagspeech39,boss6在无双死亡时说话
	var 39 0
	set_pworld_var 116 32 @39 1
	;boss3flagspeech40,boss3在龙死亡时说话
	var 40 0
	set_pworld_var 120 32 @40 1
	;boss3npcmemid41,boss3npc的mid
	var 41 0
	set_pworld_var 124 32 @41 1
	;boss3stage42,boss3战斗阶段
	var 42 0
	set_pworld_var 128 32 @42 1
endproc
;刷怪
;第一层
proc 101
	;警报器
	new_mon 206303 1 18716 9953 0 0 0 1 0
	new_mon 206304 1 20023 10008 0 0 0 1 0
	new_mon 206305 1 18188 10955 0 0 0 1 0
	new_mon 206306 1 20553 11026 0 0 0 1 0
	new_mon 206307 1 18721 12025 0 0 0 1 0
	new_mon 206308 1 19997 12049 0 0 0 1 0
;	new_mon 206268 1 19364 12428 0 0 0 1 0
	;升降梯
	NEW_NPC 1 206240 19422 15163 90 pw105lift1.mac
	NEW_NPC 1 206241 19346 20771 0 pw105lift2.mac
;	NEW_NPC 1 350064 19628 30660 0 pw105lift3.mac	
	NEW_NPC 1 206242 19499 33267 0 pw105lift4.mac
endproc
;一层boss挂了
proc 102
	;警报器
;	delete_mon_by_defid 206268 1
;	;阻挡
;	delete_mon_by_defid 261039 1
	;测试tick用的,阶段
	VAR 16 1
	set_pworld_var 24 32 @16 1
endproc
;一层招boss
proc 103
	
	new_mon 206272 1 19344 10978 0 0 0 1 0
endproc
;刷怪
;第二层
proc 201
	;一个密码机器人npc
	NEW_NPC 1 206238 18800 29200 0 pw105pwd.mac
	;一个隐藏关开门npc	
;	NEW_NPC 1 284052 20104 30534 0 pw105hidedoor.mac
endproc
;第二层刷密码方块
proc 202
	;删红环
	;delete_mon_by_defid 284049 1
	add_sys_msg_start
	add_sys_msg 密码破译开始！注意看那个与众不同的，然后站进去！
	set_sys_msg_mode 3 0 0
	map_msg_info
	strvar r
	rand r 9
	strvar x
	strvar y
	strvar posX
	strvar posY
	strvar offset
	;蓝色284050
	;红色284049
	;NEW_LIFE_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #朝向 #生存时间 #返回标志 #第一只怪物的实例ID
	strvar id 
	strvar t
	strvar i
	var i 0
	var t 10
	var x -1
	
	while x < 2
		var y -1
		while y < 2
			var posX 19620
			var posY 29736
			var offset 350
			mul offset @x
			add posX @offset
			var offset 350 
			mul offset @y
			add posY @offset
			var id 284049
			if i = @r
				var id 284050
				if i != 0
					var id 284096
					add id @i
					sub id 1
				endif
				NEW_LIFE_MON @id 1 @posX @posY 0 0 0 0 @t 1 0
			else
				if i != 0
					var id 284088
					add id @i
					sub id 1
				endif
				new_mon @id 1 @posX @posY 0 0 0 1 0
			endif	
			add i 1	
			add y 1
		endwhile
		
		add x 1
		
		;
	endwhile
endproc
;第二层密码过关
proc 203
	;飞碟
	NEW_MON_MACHINE 1 1460058 0 1 19620 29736 0
	;删阻挡
	delete_mon_by_defid 261040 1
	add_sys_msg_start
	add_sys_msg 恭喜你们，大门打开了！你们距离维奇教授的实验室又近了一步！
	set_sys_msg_mode 3 0 0
	map_msg_info
	;测试tick用的,阶段
	VAR 16 2
	set_pworld_var 24 32 @16 1
	
	del_npc 206238 1
endproc
;刷怪
;第三层
proc 301
;	call 304
;	;5层怪组合
;	strvar i onex oney twox twoy
;	var i 0
;	var onex 16112
;	var oney 30125
;	var twox 16090
;	var twoy 29306
;	while i < 1
;		
;		;第一行
;		new_mon 206247 1 @onex @oney 0 0 0 1 0
;		add onex -1044
;		add oney -3
;
;		new_mon 206249 1 @onex @oney 0 0 0 1 0
;		add onex -1044
;		add oney -3
;
;		new_mon 206248 1 @onex @oney 0 0 0 1 0
;		
;
;		add onex 0
;		add oney -443
;		if i = 0
;			new_mon 206246 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206251 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206253 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206255 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206257 1 @onex @oney 0 0 0 1 0
;		endif		
;		
;		add onex -1044
;		add oney -3
;
;		add onex -1044
;		add oney -3
;
;		if i = 0
;			new_mon 206245 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206250 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206252 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206254 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206256 1 @onex @oney 0 0 0 1 0
;		endif
;		
;		sub onex 0
;		sub oney -443
;
;		add onex -1044
;		add oney -3
;
;		;第二行
;		new_mon 206247 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		new_mon 206249 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		new_mon 206248 1 @twox @twoy 0 0 0 1 0
;		add twox -1044
;		add twoy -3
;
;		add twox -1044
;		add twoy -3
;
;		add twox -1044
;		add twoy -3
;
;		add i 1
;	endwhile
;
;	var onex 12106
;	var oney 30144
;	var twox 9020
;	var twoy 26527
;	
;	while i < 5
;		
;		;第一行
;		new_mon 206247 1 @onex @oney 0 0 0 1 0
;		add onex 4
;		add oney -952
;
;		new_mon 206249 1 @onex @oney 0 0 0 1 0
;		add onex 4
;		add oney -952
;		add onex -1
;
;
;		new_mon 206248 1 @onex @oney 0 0 0 1 0
;		
;
;		add onex 457
;		add oney -1
;		if i = 0
;			new_mon 206246 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206251 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206253 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206255 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206257 1 @onex @oney 0 0 0 1 0
;		endif		
;		
;		add onex 4
;		add oney -952
;
;		add onex 4
;		add oney -952
;
;		if i = 0
;			new_mon 206245 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 1
;			new_mon 206250 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 2
;			new_mon 206252 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 3
;			new_mon 206254 1 @onex @oney 0 0 0 1 0
;		endif
;		if i = 4
;			new_mon 206256 1 @onex @oney 0 0 0 1 0
;		endif
;		
;		sub onex 457
;		sub oney -1
;
;		add onex 4
;		add oney -952
;
;		;第二行
;		new_mon 206247 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		new_mon 206249 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		new_mon 206248 1 @twox @twoy 0 0 0 1 0
;		add twox 4
;		add twoy -952
;
;		add twox 4
;		add twoy -952
;
;		add twox 4
;		add twoy -952
;
;		add i 1
;	endwhile
	
	;link怪
;	new_mon 206324 1 8700 10700 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	new_mon 206324 1 7700 10200 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	new_mon 206234 1 10000 9500 0 0 0 1 2
;
;	SET_MON_GROUP 1 @2 31
;	LINK_MON 1 31
;
;	new_mon 206324 1 7200 9500 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	new_mon 206324 1 9600 10100 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	new_mon 206234 1 8608 14420 0 0 0 1 2
;	SET_MON_GROUP 1 @2 32
;	LINK_MON 1 32
;
;
;	new_mon 206324 1 9060 10947 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;
;	new_mon 206324 1 8200 10789 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;
;	new_mon 206234 1 8618 11678 0 0 0 1 2
;	SET_MON_GROUP 1 @2 33
;	LINK_MON 1 33
	;boss
	new_mon 206274 1 8651 9133 0 0 0 1 0
;::substr206322小飞机怪的id::284051::206322
endproc
;清通道
proc 302
	;ufoflag33,ufo重生标志,0重生,1不重生
	var 33 1
	set_pworld_var 92 32 @33 1
	delete_mon_by_defid 206247 1	
	delete_mon_by_defid 206249 1
	delete_mon_by_defid 206248 1
	delete_mon_by_defid 206322 1
	delete_mon_by_defid 1460058 1		
endproc
;清右通道的阻挡
proc 303
	;播放动画
	;删阻挡
	delete_mon_by_defid 261041 1
	;种传送球
	NEW_NPC 1 360056 8641 7584 0 pw105trans.mac
	
	;测试tick用的,阶段
	VAR 16 3
	set_pworld_var 24 32 @16 1
;	add_sys_msg_start
;	add_sys_msg 亚瑟:这并不算什么，在亚瑟的眼中，你们距离死亡又近了一步！ 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
	;boss3npcmemid41,boss3npc的mid
	var 41 0
	get_pworld_var 124 32 1 41
	if 41 > 0
		mon_speech @41 1 2 3 这并不算什么，在亚瑟的眼中，你们距离死亡又近了一步！
	endif
endproc
proc 304
	;
	;第一行
	new_mon 206247 1 16112 30125 0 0 0 1 0
	new_mon 206247 1 8202 20617 0 0 0 1 0
	new_mon 206247 1 8204 24538 0 0 0 1 0
	new_mon 206247 1 8658 28503 0 0 0 1 0
	new_mon 206247 1 12106 30144 0 0 0 1 0
	new_mon 206249 1 15072 30139 0 0 0 1 0
	new_mon 206249 1 8191 19634 0 0 0 1 0
	new_mon 206249 1 8175 23578 0 0 0 1 0
	new_mon 206249 1 8257 27490 0 0 0 1 0
	new_mon 206249 1 11227 30138 0 0 0 1 0
	new_mon 206248 1 14113 30141 0 0 0 1 0
	new_mon 206248 1 8185 18707 0 0 0 1 0
	new_mon 206248 1 8187 22536 0 0 0 1 0
	new_mon 206248 1 8186 26541 0 0 0 1 0
	new_mon 206248 1 10161 29841 0 0 0 1 0
	;第二行
	new_mon 206247 1 16112 29261 0 0 0 1 0
	new_mon 206247 1 9080 20588 0 0 0 1 0
	new_mon 206247 1 9069 24516 0 0 0 1 0
	new_mon 206247 1 9404 28071 0 0 0 1 0
	new_mon 206247 1 12140 29289 0 0 0 1 0
	new_mon 206249 1 15089 29256 0 0 0 1 0
	new_mon 206249 1 9076 19625 0 0 0 1 0
	new_mon 206249 1 9075 23559 0 0 0 1 0
	new_mon 206249 1 9111 27303 0 0 0 1 0
	new_mon 206249 1 11275 29254 0 0 0 1 0
	new_mon 206248 1 14131 29288 0 0 0 1 0
	new_mon 206248 1 9063 18674 0 0 0 1 0
	new_mon 206248 1 9055 22530 0 0 0 1 0
	new_mon 206248 1 9059 26532 0 0 0 1 0
	new_mon 206248 1 10494 29027 0 0 0 1 0
	var 10 45
	new_mon_by_dir 206246 1 13380 29701 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13152 29697 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13146 29274 0 0 0 @10 1 2
	new_mon_by_dir 206245 1 13165 30109 0 0 0 @10 1 2
	var 10 22
	new_mon_by_dir 206251 1 9810 29123 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9590 28991 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9752 28780 0 0 0 @10 1 2
	new_mon_by_dir 206250 1 9372 29204 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206253 1 8632 25673 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8637 25494 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8346 25483 0 0 0 @10 1 2
	new_mon_by_dir 206252 1 8940 25500 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206255 1 8631 21805 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8640 21630 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8328 21641 0 0 0 @10 1 2
	new_mon_by_dir 206254 1 8931 21644 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 206257 1 8642 17830 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8640 17640 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8327 17659 0 0 0 @10 1 2
	new_mon_by_dir 206256 1 8933 17634 0 0 0 @10 1 2
endproc
;刷怪
;第四层
proc 401
	
	;boss npc
	NEW_NPC 1 317013 34553 29488 45 pw105boss3.mac
	;boss3npcmemid41,boss3npc的mid
	var 41 @1
	set_pworld_var 124 32 @41 1
endproc
;区域1招怪
proc 402
;	new_mon 206323 1 21967 29700 0 0 0 1 0
	destroy_mon_by_defid 1 206282
endproc
;区域2招怪
proc 403
;	new_mon 206323 1 24400 29716 0 0 0 1 0
	destroy_mon_by_defid 1 206284
endproc
;区域3招怪
proc 404
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206285
endproc
;boss3 对话后,关门
proc 405
	;删ufo
	delete_mon_by_defid 1460058 1	
	;刷临时机械
	;缺接口
;	::substr14600563号房里的临时机械. 防御型::1460056::1460056
;	::substr14600573号房里的临时机械. 攻击型::1460057::1460057
	var 10 160
	new_mon_machine_by_dir 1 1460056 0 1 33324 30219 0 @10
	
	var 10 164
	new_mon_machine_by_dir 1 1460057 0 1 33589 30458 0 @10
	
	var 10 168
	new_mon_machine_by_dir 1 1460057 0 1 33817 30657 0 @10
	var 10 172
	new_mon_machine_by_dir 1 1460057 0 1 34096 30813 0 @10
	var 10 176
	new_mon_machine_by_dir 1 1460057 0 1 34401 30900 0 @10
	var 10 180
	new_mon_machine_by_dir 1 1460056 0 1 34704 30864 0 @10
	;关门
	var 10 45
;	new_mon_by_dir 261039 1 31544 29058 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31544 28758 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31489 30074 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31510 29844 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31521 29618 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31515 29435 0 0 0 @10 1 2
;	new_mon_by_dir 261039 1 31544 29058 0 0 0 @10 1 2
	;boss3flagclose38,boss3脱离战斗后开门
	var 38 0
	set_pworld_var 112 32 @38 1
	;boss3flag34,boss3npc对话后计时
	var 34 0
	set_pworld_var 96 32 @34 1
	add_sys_msg_start
	add_sys_msg 选择一个临时机械。
	set_sys_msg_mode 3 0 0
	map_msg_info
;	::substr206270机甲兽怪物::206270::206270
endproc
;第一波后
proc 406
	;删npc
	;出boss
	;EFFECT_ID 0 @19 playMontion(%d,"282013_18")
	;删npc
	DEL_NPC 317013 1	
	;出boss
	var 10 0
	NEW_MON_BY_DIR 206278 1 34553 29488 0 0 0 10 1 2        ;工程师/乘骑（BOSS）
	delete_mon_by_defid 1460057 1
	delete_mon_by_defid 1460056 1
	;boss3stage42,boss3战斗阶段
	var 42 2
	set_pworld_var 128 32 @42 1
endproc
;四层boss死亡后
proc 407
	;删阻挡
	delete_mon_by_defid 261042 1
	delete_mon_by_defid 261039 1
	
	;测试tick用的,阶段
	VAR 16 4
	set_pworld_var 24 32 @16 1
endproc
;区域4招怪
proc 408
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206286
endproc
;区域5招怪
proc 409
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206287
endproc
;区域6招怪
proc 410
;	new_mon 206323 1 26672 29678 0 0 0 1 0
	destroy_mon_by_defid 1 206289
endproc
;招boss3旁边的怪
proc 411
	;放狗
	;出第一波
	new_mon 206270 1 35500 29200 0 0 0 1 0
	new_mon 206270 1 35300 28900 0 0 0 1 0
	new_mon 206270 1 34900 28600 0 0 0 1 0
	new_mon 206270 1 35600 29600 0 0 0 1 0
	new_mon 206270 1 34600 28600 0 0 0 1 0	
	add_sys_msg_start
	add_sys_msg 终结者出现了！
	set_sys_msg_mode 3 0 0
	map_msg_info
	;boss3stage42,boss3战斗阶段
	var 42 1
	set_pworld_var 128 32 @42 1
endproc
;挑战boss3失败后开门
proc 412
	DEL_NPC 317013 1	
	destroy_mon_by_defid 1 206278
	destroy_mon_by_defid 1 1460057
	destroy_mon_by_defid 1 1460056
	destroy_mon_by_defid 1 206270
	destroy_mon_by_defid 1 261039
	destroy_mon_by_defid 1 206276
	;boss3对话标记,0未触发过,1触发过
	VAR 20 0
	SET_PWORLD_VAR 40 32 @20 1
	;招npc
	CALL 401
endproc
;刷怪
;第五层
proc 501
;	new_mon 206243 1 34628 23065 0 0 0 1 0
;	new_mon 206243 1 34587 15626 0 0 0 1 0
;	new_mon 206243 1 34537 16573 0 0 0 1 0
;	new_mon 206243 1 34610 17646 0 0 0 1 0
;	new_mon 206243 1 34570 18796 0 0 0 1 0
;	new_mon 206243 1 34525 19952 0 0 0 1 0
;	new_mon 206243 1 34517 21130 0 0 0 1 0
;	new_mon 206243 1 34537 22174 0 0 0 1 0
	strvar memid
	new_mon 206280 1 34610 14087 0 0 0 1 memid
	TARGET_ADD_STATUS 3 @memid 3006 1 30 0 
	new_mon 206258 1 34610 14087 0 0 0 1 0
endproc
;删怪
;第五层
proc 502
	;删毒气怪
	DELETE_MON_BY_DEFID 206258 1 
	;各种招的怪
	delete_mon_by_defid 261035 1
	delete_mon_by_defid 261036 1
	delete_mon_by_defid 261037 1
	delete_mon_by_defid 261038 1
	delete_mon_by_defid 282037 1
	delete_mon_by_defid 282038 1
	delete_mon_by_defid 282039 1
	delete_mon_by_defid 282040 1
	
	;种传送球
	NEW_NPC 1 333048 35000 13400 0 pw105out.mac
	;开门
	delete_mon_by_defid 206317 1
	PWORLD_BOX_START 0
	pworld_over
	OPEN_PWORLD_AWARD_UI
	
	
endproc
;关门
;第五层
proc 503
	return
	;删ufo
	delete_mon_by_defid 1460058 1	
	var 10 0
	new_mon_by_dir 206317 1 34665 15413 0 0 0 @10 1 2
	new_mon_by_dir 206317 1 34254 15407 0 0 0 @10 1 2
	new_mon_by_dir 206317 1 34864 15438 0 0 0 @10 1 2
	;boss5flag36,boss5脱离战斗后开门
	var 36 0
	set_pworld_var 104 32 @36 1
endproc
;开门
;第五层
proc 504
	DELETE_MON_BY_DEFID 206317 1 
	;boss5flag35,boss5战斗后关门
	var 35 0
	set_pworld_var 100 32 @35 1
endproc
;6层删阻挡
proc 600
	;删阻挡
	delete_mon_by_defid 261043 1
	;测试tick用的,阶段
	VAR 16 5
	set_pworld_var 24 32 @16 1
endproc
;刷怪
;第六层
proc 601
	new_mon 284066 1 19526 36649 0 0 0 1 0
	new_mon 206281 1 19526 36649 0 0 0 1 0
endproc
;各种阻挡 
proc 701
	var 10 0
	new_mon_by_dir 261039 1 19430 12754 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 18814 12785 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19623 12762 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 18608 12761 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 20167 12768 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19830 12770 0 0 0 @10 1 2
	new_mon_by_dir 261039 1 19060 12771 0 0 0 @10 1 2
	var 10 45
	new_mon_by_dir 261040 1 18599 29710 0 0 0 @10 1 2
;	new_mon_by_dir 261040 1 18550 29959 0 0 0 @10 1 2
;	new_mon_by_dir 261040 1 18543 29481 0 0 0 @10 1 2
	var 10 45
;	new_mon_by_dir 261041 1 20638 29712 0 0 0 @10 1 2
;	new_mon_by_dir 261041 1 20674 29991 0 0 0 @10 1 2
;	new_mon_by_dir 261041 1 20687 29414 0 0 0 @10 1 2
	var 10 0
	new_mon_by_dir 261042 1 34611 26447 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 33980 26399 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 34261 26404 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 34925 26434 0 0 0 @10 1 2
	new_mon_by_dir 261042 1 35179 26465 0 0 0 @10 1 2
;	new_mon_by_dir 261042 1 31449 29750 0 0 0 @10 1 2
;	var 10 0
;	new_mon_by_dir 261043 1 19624 30486 0 0 0 @10 1 2
;	var 10 45
;	new_mon_by_dir 261043 1 19804 30602 0 0 0 @10 1 2
;	new_mon_by_dir 261043 1 19419 30592 0 0 0 @10 1 2
endproc
;重置,调试用
PROC 10
	pworld_again
ENDPROC
proc 11
        OUT_PWORLD_BY_SINGLE
endproc
PROC 12
	pworld_over
ENDPROC
PROC 13
	delete_mon_by_defid 261039 1
	delete_mon_by_defid 261040 1
	delete_mon_by_defid 261041 1
	delete_mon_by_defid 261042 1
	delete_mon_by_defid 261043 1
	delete_mon_by_defid 206245 1
	delete_mon_by_defid 206250 1
	delete_mon_by_defid 206252 1
	delete_mon_by_defid 206254 1
	delete_mon_by_defid 206256 1
ENDPROC
proc 14
	;密码光环站对次数
	VAR 14 6
	set_pworld_var 16 32 @14 1
endproc
