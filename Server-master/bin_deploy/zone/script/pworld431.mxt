;跨服公会战副本    ,公会跨服战人少的那个,死亡游戏


proc 1
	;变量初始化
	;monmemid10,枢纽怪memid 1
	var 10 0
	set_pworld_var 0 32 @10 1
	;monmemid11,枢纽怪memid 2
	var 11 0
	set_pworld_var 4 32 @11 1
	;monmemid12,枢纽怪memid 3
	var 12 0
	set_pworld_var 8 32 @12 1
	;camp1buffcool12,阵营1buff冷却时间
	var 12 0
	set_pworld_var 8 32 @12 1
	;camp2buffcool13,阵营2buff冷却时间
	var 13 0
	set_pworld_var 12 32 @13 1
	;camp3buffcool14,阵营3buff冷却时间
	var 14 0
	set_pworld_var 16 32 @14 1
	;camp1fly1buy15,阵营1飞机1购买标记
	var 15 1
	set_pworld_var 20 32 @15 1
	;camp1fly2buy16,阵营1飞机2购买标记
	var 16 0
	set_pworld_var 24 32 @16 1
	;camp1fly3buy17,阵营1飞机3购买标记
	var 17 0
	set_pworld_var 28 32 @17 1
	;camp1fly4buy18,阵营1飞机4购买标记
	var 18 0
	set_pworld_var 32 32 @18 1
	;camp1fly5buy19,阵营1飞机5购买标记
	var 19 0
	set_pworld_var 36 32 @19 1
	;camp2fly1buy20,阵营2飞机1购买标记
	var 20 1
	set_pworld_var 40 32 @20 1
	;camp2fly2buy21,阵营2飞机2购买标记
	var 21 0
	set_pworld_var 44 32 @21 1
	;camp2fly3buy22,阵营2飞机3购买标记
	var 22 0
	set_pworld_var 48 32 @22 1
	;camp2fly4buy23,阵营2飞机4购买标记
	var 23 0
	set_pworld_var 52 32 @23 1
	;camp2fly5buy24,阵营2飞机5购买标记
	var 24 0
	set_pworld_var 56 32 @24 1
	;camp3fly1buy25,阵营3飞机1购买标记
	var 25 1
	set_pworld_var 60 32 @25 1
	;camp3fly2buy26,阵营3飞机2购买标记
	var 26 0
	set_pworld_var 64 32 @26 1
	;camp3fly3buy27,阵营3飞机3购买标记
	var 27 0
	set_pworld_var 68 32 @27 1
	;camp3fly4buy28,阵营3飞机4购买标记
	var 28 0
	set_pworld_var 72 32 @28 1
	;camp3fly5buy29,阵营3飞机5购买标记
	var 29 0
	set_pworld_var 76 32 @29 1
	;battlestat30,战斗阶段|0初始阶段 1准备阶段 2开始阶段
	var 30 1
	set_pworld_var 80 32 @30 1
	;camp1buffcoolreduce31,阵营1buff缩短冷却时间使用次数
	var 31 0
	set_pworld_var 84 32 @31 1
	;camp2buffcoolreduce32,阵营2buff缩短冷却时间使用次数
	var 32 0
	set_pworld_var 88 32 @32 1
	;camp3buffcoolreduce33,阵营3buff缩短冷却时间使用次数
	var 33 0
	set_pworld_var 92 32 @33 1
	;camp1die34,阵营1枢纽死亡标记
	var 34 0
	set_pworld_var 96 32 @34 1
	;camp2die35,阵营2枢纽死亡标记
	var 35 0
	set_pworld_var 100 32 @35 1
	;camp3die36,阵营3枢纽死亡标记
	var 36 0
	set_pworld_var 104 32 @36 1
	;camp1num37,阵营1玩家进入次数
	var 37 0
	set_pworld_var 108 32 @37 1
	;camp2num38,阵营2玩家进入次数
	var 38 0
	set_pworld_var 112 32 @38 1
	;camp3num39,阵营3玩家进入次数
	var 39 0
	set_pworld_var 116 32 @39 1

	;各阵营飞行点
	;NEW_CAMP_NPC 1 350103 10089 13864 0 1 npc350103.mac
	NEW_NPC 1 350103 10089 13864 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 16
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 10412 13694 0 1 npc350103.mac
	NEW_NPC 1 350103 10412 13694 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 17
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 9361 14010 0 1 npc350103.mac
	NEW_NPC 1 350103 9361 14010 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 18
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350103 9019 13978 0 1 npc350103.mac
	NEW_NPC 1 350103 9019 13978 0 npc350103.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 19
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17413 22613 0 2 npc350104.mac
	NEW_NPC 1 350104 17413 22613 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 21
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17727 23315 0 2 npc350104.mac
	NEW_NPC 1 350104 17727 23315 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 22
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17705 23763 0 2 npc350104.mac
	NEW_NPC 1 350104 17705 23763 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 23
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350104 17167 22373 0 2 npc350104.mac
	NEW_NPC 1 350104 17167 22373 0 npc350104.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 24
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21288 11852 0 3 npc350105.mac
	NEW_NPC 1 350105 21288 11852 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 26
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21766 11250 0 3 npc350105.mac
	NEW_NPC 1 350105 21766 11250 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 27
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 22050 11033 0 3 npc350105.mac
	NEW_NPC 1 350105 22050 11033 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 28
		set_mon_var 0 32 @10 1
	endif
	
	;NEW_CAMP_NPC 1 350105 21150 12301 0 3 npc350105.mac
	NEW_NPC 1 350105 21150 12301 0 npc350105.mac
	if @1 > 0
		set_mon_curr @1 2
		;monfly10,飞行器已购买标记|副本变量索引
		var 10 29
		set_mon_var 0 32 @10 1
	endif
	

	;各阵营枢纽守护者
	;NEW_DYN_MON 1 303070 1 8106 12167 0 0 0 2 1 0 0
	NEW_CAMP_NPC 1 303070 8106 12167 135 1 npc303070.mac
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 1 0
;	endif
	;NEW_DYN_MON 1 303071 1 15919 25655 0 0 0 2 2 0 0
	NEW_CAMP_NPC 1 303071 15919 25655 0 2 npc303071.mac
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 2 0
;	endif
	;NEW_DYN_MON 1 303072 1 24000 10950 0 0 0 2 3 0 0
	NEW_CAMP_NPC 1 303072 24000 10950 60 3 npc303072.mac
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
;	if @1 > 0
;		SET_CAMP_MON_MAP_SHOW 2 @1 3 0
;	endif

	;各阵营枢纽
	;NEW_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #返回标志 #第一只怪物的实例ID
	NEW_MON 261196 1 9285 12319 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
	;SET_MON_CAMP_IDX #返回值 #实例ID #阵营索引
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 1
		SET_CAMP_MON_MAP_SHOW 2 @3 1 0
	endif
	;NEW_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #返回标志 #第一只怪物的实例ID
	NEW_MON 261197 1 16041 23527 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
	;SET_MON_CAMP_IDX #返回值 #实例ID #阵营索引
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 2
		SET_CAMP_MON_MAP_SHOW 2 @3 2 0
	endif
	;NEW_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #返回标志 #第一只怪物的实例ID
	NEW_MON 261198 1 22734 12442 0 0 0 1 3 
	;SET_CAMP_MON_MAP_SHOW #返回标志 #怪物MemID #阵营 #类型
	;SET_MON_CAMP_IDX #返回值 #实例ID #阵营索引
	if 1 = 0
		SET_MON_CAMP_IDX 2 @3 3
		SET_CAMP_MON_MAP_SHOW 2 @3 3 0
	endif

	add_sys_msg_start
	add_sys_msg 战斗30秒后开始 
	set_sys_msg_mode 3 0 0
	map_msg_info

	SET_PWORLD_TIMER 1 30
	;各阵营阻挡
	call 100

	
endproc

;
proc 3
;	strvar campid num
;	battle_min_camp_player_num campid num
;	if num <= 0 and campid > 0
;	endif
endproc

; 怪物死亡事件
proc 4
	strvar defid livenum livecamp
	var livenum 0
;	var diecamp 0
	var livecamp 0
	;GET_NPC_NUM #NpcID #返回标志 #数量
	;GET_MON_INFO #返回标志 #定义id #实例id #血量 #地图id #地图实例 #坐标X #Y 
	get_mon_info 1 defid 0 0 0 0 0 0


	;枢纽怪 1
	;GET_MON_NUM_BY_DEFID #怪物的定义ID #返回标志
;	GET_MON_NUM_BY_DEFID 261196 3
;	if defid = 261196 or 3 < 1
;		add dienum 1
;		add diecamp 1
;	endif 
	if defid = 261196 
		;camp1die34,阵营1枢纽死亡标记
		var 34 0
		get_pworld_var 96 32 1 34
		add 34 1
		;camp1die34,阵营1枢纽死亡标记
		var 34 @34;
		set_pworld_var 96 32 @34 1
		
		DEL_NPC 303070 1
	endif 

	;枢纽怪 2
	;GET_MON_NUM_BY_DEFID #怪物的定义ID #返回标志
;	GET_MON_NUM_BY_DEFID 261197 3
;	if defid = 261197 or 3 < 1
;		add dienum 1
;		add diecamp 2
;	endif 
	if defid = 261197 
		;camp2die35,阵营2枢纽死亡标记
		var 35 0
		get_pworld_var 100 32 1 35
		add 35 1
		;camp2die35,阵营2枢纽死亡标记
		var 35 @35;
		set_pworld_var 100 32 @35 1
		
		DEL_NPC 303071 1
	endif 

	;枢纽怪 3
	;GET_MON_NUM_BY_DEFID #怪物的定义ID #返回标志
;	GET_MON_NUM_BY_DEFID 261198 3
;	if defid = 261198 or 3 < 1
;		add dienum 1
;		add diecamp 3
;	endif 
	if defid = 261198 
		;camp3die36,阵营3枢纽死亡标记
		var 36 0
		get_pworld_var 104 32 1 36
		add 36 1
		;camp3die36,阵营3枢纽死亡标记
		var 36 @36;
		set_pworld_var 104 32 @36 1
		
		DEL_NPC 303072 1
	endif 
	
	;camp1die34,阵营1枢纽死亡标记
	var 34 0
	get_pworld_var 96 32 1 34
	if 34 = 0
		add livenum 1
		var livecamp 1
	endif
	
	;camp2die35,阵营2枢纽死亡标记
	var 35 0
	get_pworld_var 100 32 1 35
	if 35 = 0
		add livenum 1
		var livecamp 2
	endif
	
	;camp3die36,阵营3枢纽死亡标记
	var 36 0
	get_pworld_var 104 32 1 36
	if 36 = 0
		add livenum 1
		var livecamp 3
	endif
	if livenum = 1

		set_battle_win 1 @livecamp
		PWORLD_OVER
;		add_sys_msg_start
;		add_sys_msg win:$@livecamp$
;		set_sys_msg_mode 1 0 0
;		map_msg_info
	endif
	
	
	
;	add_sys_msg_start
;	add_sys_msg die:$@defid$ live:$@livenum$
;	set_sys_msg_mode 1 0 0
;	map_msg_info

endproc

;玩家进入事件
proc 5

	
	;提示
	strvar begintime
	get_pworld_timer begintime
	if begintime > 0

		add_sys_msg_start
		add_sys_msg 战斗$@begintime$秒后开始  
		set_sys_msg_mode 3 0 0
		msg_info
	endif

	;帮助
	EFFECT 2 1 SetMessageTips(320)

	;枢纽小地图显示
	;CAMP_MON_MAP_SHOW_ACTION #返回标志 #阵营 #类型 #广播类型
	CAMP_MON_MAP_SHOW_ACTION 1 0 1 0

	strvar playercamp
	GET_ATTACKER_CAMP 1 playercamp
	;玩家进入次数
	if playercamp = 1 
		;camp1num37,阵营1玩家进入次数
		var 37 0
		get_pworld_var 108 32 1 37
		add 37 1
		;camp1num37,阵营1玩家进入次数
		
		set_pworld_var 108 32 @37 1
	endif 
	;玩家进入次数
	if playercamp = 2 
		;camp2num38,阵营2玩家进入次数
		var 38 0
		get_pworld_var 112 32 1 38
		add 38 1
		;camp2num38,阵营2玩家进入次数
		
		set_pworld_var 112 32 @38 1
	endif 
	;玩家进入次数
	if playercamp = 3 
		;camp3num39,阵营3玩家进入次数
		var 39 0
		get_pworld_var 116 32 1 39
		add 39 1
		;camp3num39,阵营3玩家进入次数
		
		set_pworld_var 116 32 @39 1
	endif 
endproc

;倒计时事件
proc 7
	;删阻挡
	delete_mon_by_defid 294008 1	

	add_sys_msg_start
	add_sys_msg 战斗开始 
	set_sys_msg_mode 3 0 0
	map_msg_info
	;进入战斗状态
	;battlestat30,战斗阶段|0初始阶段 1准备阶段 2开始阶段
	var 30 2
	set_pworld_var 80 32 @30 1
endproc

proc 100
	new_mon_by_dir 294008 1 11131 13490 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11252 13340 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11353 13201 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11423 13062 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11505 12938 0 0 0 30 1 2
	new_mon_by_dir 294008 1 11557 12834 0 0 0 30 1 2

	new_mon_by_dir 294008 1 16506 21514 0 0 0 0 1 2
	new_mon_by_dir 294008 1 16351 21502 0 0 0 0 1 2
	new_mon_by_dir 294008 1 16175 21512 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15999 21522 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15823 21532 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15647 21552 0 0 0 0 1 2
	new_mon_by_dir 294008 1 15295 21562 0 0 0 0 1 2

	new_mon_by_dir 294008 1 20701 13107 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20755 13333 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20852 13524 0 0 0 55 1 2
	new_mon_by_dir 294008 1 20949 13715 0 0 0 55 1 2
	new_mon_by_dir 294008 1 21063 13910 0 0 0 55 1 2
endproc
