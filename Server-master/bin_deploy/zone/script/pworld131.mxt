;祭祖神殿 ,祭祖神殿副本
;创建事件
PROC 1	
	;设置副本变量
	call 90
	;各种怪
	call 91
	;各种阻挡
	call 92
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
	;地图技能开关
	get_pworld_var 4 32 1 11
	;计时
	get_pworld_var 0 32 1 10
	add 10 1
	set_pworld_var 0 32 @10 1
	;1分钟
	mod 10 30
	if 10 = 0 and 11 = 1
		call 103   ;副本进度提示
	endif

	get_pworld_var 8 32 1 12
	;刷怪中
	;灵魂流波次计时
	get_pworld_var 12 32 1 13
		
	if 12 = 5 
		strvar ret num
		var ret 0
		
		GET_MON_NUM_BY_DEFID 205280 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205281 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205282 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205288 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205287 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205289 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205290 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205291 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205292 num
		add ret @num
		
		strvar time
		var time @13
		add 13 1
		set_pworld_var 12 32 @13 1
		
		;灵魂流波次
		get_pworld_var 24 32 1 16
		strvar num 
		var num 2
		if 16 = 10
			var num 0
		endif

		if ret <= @num or time > 60
			;刷灵魂流
			if num = 0 
				;第10波要等小怪全死光
				if ret <= 0
					call 202 
				endif				
			else
				call 202
			endif			
		endif

	endif
	if 12 = 4
		call 201
		add 12 1
		;进入刷怪阶段
		set_pworld_var 8 32 @12 1
	endif
	
	;祭台满血了
	if 12 = 6
		call 204
		add 12 1
		
		set_pworld_var 8 32 @12 1
	endif
	
	;boss5是否归位了
	get_pworld_var 68 32 1 27
	if 27 > 0
		call 301
		VAR 27 0
		set_pworld_var 68 32 @27 1
	endif
	
	;close28,关闭倒计时
	var 28 0
	get_pworld_var 72 32 1 28
	if 28 > 0
		add 28 1
		;close28,关闭倒计时		
		set_pworld_var 72 32 @28 1
	endif
	if 28 > 4
		add_sys_msg_start
		add_sys_msg 副本已关闭,请从传送门离开 
		set_sys_msg_mode 1 0 0
		map_msg_info
		new_npc 1 333045 14729 13113 0 pw105out.mac
		pworld_over
	endif	
ENDPROC

PROC 4
	strvar x y id hp mid
	GET_MON_INFO 0 id 0 hp 0 0 x y
	
	;门被打开了
	If id = 205316
		call 101
	endif
	;boss4挂了
	if id = 205283 and hp < 1
		call 203
		
	endif
	;boss5挂了
	if id = 201554 and hp < 1
		call 301
	endif
ENDPROC
;设置副本变量
proc 90
	;设置副本变量。
	;10计时数 
	var 10 0
	set_pworld_var 0 32 @10 1
	;地图技能开关
	var 11 0
	set_pworld_var 4 32 @11 1
	;灵魂流开关计数
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;灵魂流波次计时,波次*间隔/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;祭坛的实例id
	VAR 14 0
	set_pworld_var 16 32 @14 1
	;boss4的实例id
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;灵魂流波次
	VAR 16 0
	set_pworld_var 24 32 @16 1
	;逻辑区域1标记,0未触发过,1触发过
	VAR 17 0
	set_pworld_var 28 32 @17 1
	;机械1刷新标记,0未触发过,1触发过
	VAR 18 0
	set_pworld_var 32 32 @18 1
	;机械2刷新标记,0未触发过,1触发过
	VAR 19 0
	set_pworld_var 36 32 @19 1
	;机械3刷新标记,0未触发过,1触发过
	VAR 20 0
	set_pworld_var 40 32 @20 1
	;机械1掉落位置x
	VAR 21 0
	set_pworld_var 44 32 @21 1
	;机械1掉落位置y
	VAR 22 0
	set_pworld_var 48 32 @22 1
	;机械2掉落位置x
	VAR 23 0
	set_pworld_var 52 32 @23 1
	;机械2掉落位置y
	VAR 24 0
	set_pworld_var 56 32 @24 1
	;机械3掉落位置x
	VAR 25 0
	set_pworld_var 60 32 @25 1
	;机械3掉落位置y
	VAR 26 0
	set_pworld_var 64 32 @26 1
	;boss5是否归位了
	VAR 27 0
	set_pworld_var 68 32 @27 1
	;close28,关闭倒计时
	var 28 0
	set_pworld_var 72 32 @28 1
	;shadow29,暗影屏障提示
	var 29 0
	set_pworld_var 76 32 @29 1
	;count30,祭祀计数
	var 30 0
	set_pworld_var 80 32 @30 1
	;level31,进度阶段
	var 31 1
	set_pworld_var 84 32 @31 1
	;shadow32,boss暗影屏障提示
	var 32 0
	set_pworld_var 88 32 @32 1
	;pworld33,是否pworldover了,1副本没结束,0结束了
	var 33 1
	set_pworld_var 92 32 @33 1
endproc

;各种怪
proc 91

	;三个怪的巡逻队
	new_mon 205276 1 11586 13476 0 203 0 1 2
	SET_MON_GROUP 1 @2 22
	new_mon 205274 1 11536 13476 0 204 0 1 2
	SET_MON_GROUP 1 @2 22
	new_mon 205275 1 11486 13476 0 205 0 1 2
	SET_MON_GROUP 1 @2 22

	;三个怪的巡逻队
	new_mon 205276 1 13442 16279 0 209 0 1 2
	SET_MON_GROUP 1 @2 24
	new_mon 205274 1 13392 16279 0 210 0 1 2
	SET_MON_GROUP 1 @2 24
	new_mon 205275 1 13342 16279 0 211 0 1 2
	SET_MON_GROUP 1 @2 24
	
	;三个怪的巡逻队
	new_mon 205276 1 10598 18712 0 215 0 1 2
	SET_MON_GROUP 1 @2 26
	new_mon 205274 1 10548 18712 0 216 0 1 2
	SET_MON_GROUP 1 @2 26
	new_mon 205275 1 10498 18712 0 217 0 1 2
	SET_MON_GROUP 1 @2 26

	;三个怪的巡逻队
	new_mon 205276 1 11702 19716 0 218 0 1 2
	SET_MON_GROUP 1 @2 27
	new_mon 205274 1 11652 19716 0 219 0 1 2
	SET_MON_GROUP 1 @2 27
	new_mon 205275 1 11602 19716 0 220 0 1 2
	SET_MON_GROUP 1 @2 27

	;三个怪的巡逻队
	new_mon 205276 1 15265 18895 0 224 0 1 2
	SET_MON_GROUP 1 @2 29
	new_mon 205274 1 15215 18895 0 225 0 1 2
	SET_MON_GROUP 1 @2 29
	new_mon 205275 1 15165 18895 0 226 0 1 2
	SET_MON_GROUP 1 @2 29
	
	;三个怪的巡逻队
	new_mon 205276 1 18846 17368 0 230 0 1 2
	SET_MON_GROUP 1 @2 31
	new_mon 205274 1 18796 17368 0 231 0 1 2
	SET_MON_GROUP 1 @2 31
	new_mon 205275 1 18746 17368 0 232 0 1 2
	SET_MON_GROUP 1 @2 31
	
	;各种站岗的怪
	NEW_MON_BY_DIR	205273	1	10784	19165	0	0	0	35	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	8494	13862	0	0	0	139	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	8491	13598	0	0	0	139	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	14292	18929	0	0	0	41	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205271	1	14013	18923	0	0	0	31	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	21087	15690	0	0	0	39	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	20708	13913	0	0	0	31	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	20880	15293	0	0	0	33	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205270	1	8766	19036	0	0	0	40	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	8779	18824	0	0	0	41	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	13998	19144	0	0	0	24	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	14026	18719	0	0	0	35	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	15303	19467	0	0	0	27	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	22476	13990	0	0	0	45	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	20590	14276	0	0	0	36	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205272	1	9031	18968	0	0	0	51	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	16743	19221	0	0	0	5	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	15708	19136	0	0	0	27	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	17849	19169	0	0	0	5	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	9008	18749	0	0	0	47	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205270	1	10483	18831	0	0	0	40	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	10421	19161	0	0	0	27	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205271	1	10600	18979	0	0	0	42	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205273	1	10802	18896	0	0	0	38	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205271	1	10745	13276	0	0	0	120	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	10753	13400	0	0	0	114	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	9124	13828	0	0	0	121	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	10747	13536	0	0	0	129	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	9129	13498	0	0	0	126	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205270	1	15502	19191	0	0	0	23	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	15684	18933	0	0	0	28	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	16358	19004	0	0	0	7	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	16743	19000	0	0	0	4	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	18254	18893	0	0	0	5	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	17941	18906	0	0	0	1	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205272	1	15550	19388	0	0	0	23	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	16325	19210	0	0	0	20	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205273	1	16517	19310	0	0	0	7	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	18120	19126	0	0	0	9	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	20738	14352	0	0	0	43	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	22938	14060	0	0	0	37	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205271	1	20775	14085	0	0	0	42	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205270	1	20829	15691	0	0	0	40	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	20646	15401	0	0	0	41	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	22448	14275	0	0	0	40	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	20497	13963	0	0	0	42	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205272	1	21011	15473	0	0	0	50	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	20753	15139	0	0	0	43	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	22694	14382	0	0	0	47	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	20971	13938	0	0	0	48	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	22675	14188	0	0	0	49	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205272	1	22682	13996	0	0	0	49	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205273	1	20956	14174	0	0	0	41	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205273	1	22830	14194	0	0	0	39	1	2	;	黑暗魔师
	NEW_MON_BY_DIR	205270	1	17675	18913	0	0	0	0	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	16520	19084	0	0	0	0	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	8777	13993	0	0	0	135	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	8795	13725	0	0	0	135	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	8772	13425	0	0	0	135	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	10922	13491	0	0	0	135	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	10923	13323	0	0	0	135	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205272	1	10481	13437	0	0	0	135	1	2	;	黑暗猎手
	NEW_MON_BY_DIR	205277	1	8088	13786	0	0	0	135	1	2	;	萨罗弥
	NEW_MON_BY_DIR	205278	1	16535	19552	0	0	0	0	1	2	;	塞纳尔
	NEW_MON_BY_DIR	205279	1	22045	13644	0	0	0	76	1	2	;	基尔里斯
	NEW_MON_BY_DIR	205310	1	7431	13821	0	0	0	1	1	2	;	水纹结界
	NEW_MON_BY_DIR	205311	1	16483	20058	0	0	0	0	1	2	;	火焰结界
	NEW_MON_BY_DIR	205312	1	22050	13185	0	0	0	0	1	2	;	暗影结界
	NEW_MON_BY_DIR	205270	1	10708	16747	0	0	0	80	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	11173	16627	0	0	0	100	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205270	1	11595	16305	0	0	0	100	1	2	;	黑暗战士
	NEW_MON_BY_DIR	205271	1	10892	16506	0	0	0	93	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	205271	1	11319	16334	0	0	0	100	1	2	;	黑暗魔卫
	NEW_MON_BY_DIR	261058	1	14795	14792	0	0	0	0	1	2	;	暗影屏障
	NEW_MON_BY_DIR	261058	1	15369	14189	0	0	0	45	1	2	;	暗影屏障
	NEW_MON_BY_DIR	261058	1	14790	13674	0	0	0	0	1	2	;	暗影屏障
	NEW_MON_BY_DIR	261058	1	14223	14207	0	0	0	45	1	2	;	暗影屏障
	NEW_MON_BY_DIR	261059	1	14808	14228	0	0	0	0	1	2	;	黑暗能量
	NEW_MON_BY_DIR	294031	1	14801	14276	0	0	0	0	1	2	;	暗影天幕


	

endproc
;各种阻挡
proc 92
	var 10 0		;保存朝向的数值
	new_mon_by_dir 261039 1 14700 12627 0 0 0 @10 1 2
	var 10 135		;保存朝向的数值
	new_mon_by_dir 205316 1 6842 18794 0 0 0 @10 1 2
	new_mon_by_dir 294009 1 7006 18794 0 0 0 @10 1 2
endproc
;门被打开
proc 101
	;门口打破黑暗之门事件
	;删阻挡
	delete_mon_by_defid 294009 1
	;招哨兵
	strvar m1 m2 dir
	var dir 45
	new_mon_by_dir 205298 1 7451 18913 0 0 0 @dir 1 m1
	new_mon_by_dir 205302 1 7446 19079 0 0 0 @dir 1 m2

	mon_speech @m2 2 2 1 "滴滴..发现入侵者,执行毁灭程序!!!"
	;地图技能开关
	var 11 1
	set_pworld_var 4 32 @11 1
	;广播	
	add_sys_msg_start
	add_sys_msg 你们的行动打扰了黑暗议会的计划，将承受杰克.耐罗的愤怒
	set_sys_msg_mode 4 0 0
	map_msg_info	
	new_npc 1 302120 7939 20085 180 p100spy1.mac	
	add_sys_msg_start
	add_sys_msg 发现探查员菲利亚，她或许掌握着太阳神殿的相关情报 
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
;取消打雷了
proc 102
	;随机一个活着的玩家,脚下放个雷  
	strvar mid x y
	GET_RAND_PWORLD_PLAYER_ID 1 mid
	if 1 != 0
		return
	endif
	;现在不打雷
	return
	GET_ID_PLAYER_INFO @mid 3 x
	GET_ID_PLAYER_INFO @mid 4 y
	new_mon 201032 1 @x @y 0 0 0 1 0
endproc

;提示,副本进度
proc 103
;30秒判断一次副本当前进度,给出提示:”副本进度:当前阶段1/2,破坏结界1/3”.
;一直到:”副本进度:阶段1/3,破坏结界3/3” ,
;防守阶段开启后,提示:”副本进度:2/3.抵挡献祭者1/10波，失败条件:献祭者成功献祭0/10”
;如果失败,提示任务失败,删除所有怪物,刷出传送门,倒计时传出副本
;如果成功抵挡10波后,且献祭者献祭次数<10，则刷出最终BOSS,提示:”副本进度3/3,击杀阿萨曼 0/1.”
;击杀成功后提示:” 副本进度3/3,击杀阿萨曼 1/1,副本完成”
	;level31,进度阶段
	var 31 0
	get_pworld_var 84 32 1 31
	if 31 < 1
		return
	endif
	if 31 = 1
		strvar ret num
		var ret 0
		GET_MON_NUM_BY_DEFID 205310 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205311 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205312 num
		add ret @num
		
		sub ret 3
		mul ret -1
		add_sys_msg_start
		add_sys_msg 副本进度1/3,破坏结界$@ret$/3 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	;灵魂流波次
	get_pworld_var 24 32 1 16
	if 31 = 2 and 16 > 0 and 16 < 11
		;count30,祭祀计数
		var 30 0
		get_pworld_var 80 32 1 30
		add_sys_msg_start
		add_sys_msg 副本进度2/3.抵挡献祭者$@16$/10波，失败条件:献祭者成功献祭$@30$/10 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 31 = 3
		add_sys_msg_start
		add_sys_msg 副本进度3/3,击杀阿萨曼0/1 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 31 = 4
		add_sys_msg_start
		add_sys_msg 副本进度3/3,击杀阿萨曼1/1,副本完成 
		set_sys_msg_mode 1 0 0
		map_msg_info
		var 31 0
		;level31,进度阶段
		var 31 31
		set_pworld_var 84 32 @31 1
	endif
endproc

;采集物开关已全打开,进入2阶段
proc 201
	;删npc
	DEL_NPC 306023 1	
	;出boss
;	new_mon 205283 1 14851 14163 0 0 0 1 2
;	mon_speech @2 2 2 1 该死的人类,竟敢打扰我召唤伟大的影.杰克.耐罗的降临,承受我的怒火吧!
	
	NEW_MON_BY_DIR 333049 1 11131 13657 0 0 0 45 1 0
	NEW_MON_BY_DIR 333049 1 18465 14930 0 0 0 45 1 0
	
	;level31,进度阶段
	var 31 2
	set_pworld_var 84 32 @31 1
endproc
;灵魂流,新
proc 202
	;灵魂流波次
	get_pworld_var 24 32 1 16
	add 16 1
	set_pworld_var 24 32 @16 1
	if 16 < 11
		add_sys_msg_start
		add_sys_msg 第$@16$波祭祀者攻来了！守住祭坛！阻止他们的献祭！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
;		左边	13740	14011	右边	15843	14219
		effect 0 0 playEffct(13740,14011,"effect\\entity\\9571_16.ent")
		effect 0 0 playEffct(15843,14219,"effect\\entity\\9569_16.ent")
	endif
	
	if 16 = 1	
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
	endif
	if 16 = 2
		new_mon 205289 1 11362 13656 0 1040 0 0 0
		new_mon 205289 1 11274 13818 0 1041 0 0 0
		new_mon 205289 1 11275 13517 0 1042 0 0 0
		new_mon 205289 1 18210 14852 0 1050 0 0 0
		new_mon 205289 1 18310 15022 0 1051 0 0 0
		new_mon 205289 1 18409 14817 0 1052 0 0 0
	endif
	if 16 = 3
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
	endif
	if 16 = 4
		new_mon 205290 1 11362 13656 0 1040 0 0 0
		new_mon 205290 1 11274 13818 0 1041 0 0 0
		new_mon 205288 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205290 1 18210 14852 0 1050 0 0 0
		new_mon 205290 1 18310 15022 0 1051 0 0 0
		new_mon 205288 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
	endif
	if 16 = 5
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205281 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205281 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 6	
		new_mon 205291 1 11362 13656 0 1040 0 0 0
		new_mon 205291 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 11230 13682 0 1043 0 0 0
		new_mon 205288 1 11420 13505 0 1044 0 0 0
		new_mon 205291 1 18210 14852 0 1050 0 0 0
		new_mon 205291 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205280 1 18154 15044 0 1053 0 0 0
		new_mon 205288 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 7
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205288 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205288 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 8
		new_mon 205292 1 11362 13656 0 1040 0 0 0
		new_mon 205292 1 11274 13818 0 1041 0 0 0
		new_mon 205292 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 11230 13682 0 1043 0 0 0
		new_mon 205280 1 11420 13505 0 1044 0 0 0
		new_mon 205292 1 18210 14852 0 1050 0 0 0
		new_mon 205292 1 18310 15022 0 1051 0 0 0
		new_mon 205292 1 18409 14817 0 1052 0 0 0
		new_mon 205280 1 18154 15044 0 1053 0 0 0
		new_mon 205280 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 9
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205282 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205282 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 10
		
		new_mon 205289 1 11362 13656 0 1040 0 0 0
		new_mon 205291 1 11274 13818 0 1041 0 0 0
		new_mon 205292 1 11275 13517 0 1042 0 0 0
		new_mon 205289 1 18210 14852 0 1050 0 0 0
		new_mon 205291 1 18310 15022 0 1051 0 0 0
		new_mon 205292 1 18409 14817 0 1052 0 0 0
		
	endif
	if 16 = 11
		;		14754,15108
		effect 0 0 playEffct(14754,15108,"effect\\entity\\9570_16.ent")
		NEW_MON_BY_DIR 333049 1 14687 15884 0 0 0 0 1 0
		new_mon 205287 1 14687 15884 0 1060 0 0 0
		
		var 13 0
		set_pworld_var 12 32 @13 1
		;level31,进度阶段
		var 31 3
		set_pworld_var 84 32 @31 1
	endif
	
	if 16 = 1
		add_sys_msg_start
		add_sys_msg 阻止失心信徒到达黑暗祭坛！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 2
		add_sys_msg_start
		add_sys_msg 阻止苦行信徒到达黑暗祭坛！小心！他们跑得特别快！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 3
		add_sys_msg_start
		add_sys_msg 阻止失心信徒到达黑暗祭坛！小心他们的抵抗！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 4
		add_sys_msg_start
		add_sys_msg 阻止坚定信徒到达黑暗祭坛！全力集火他们！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 5
		add_sys_msg_start
		add_sys_msg 阻止失心信徒到达黑暗祭坛！小心崇尚信徒的袭击！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 6
		add_sys_msg_start
		add_sys_msg 阻止亡命信徒到达黑暗祭坛！小心他们自爆同归于尽！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 7
		add_sys_msg_start
		add_sys_msg 阻止失心信徒到达黑暗祭坛！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 8
		add_sys_msg_start
		add_sys_msg 阻止殉难信徒到达黑暗祭坛！一定要小心，他们非常强大！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 9
		add_sys_msg_start
		add_sys_msg 阻止失心信徒到达黑暗祭坛！小心虔诚信徒的袭击！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 10
		add_sys_msg_start
		add_sys_msg 阻止苦行信徒,亡命信徒,殉难信徒到达祭坛,小心他们的特殊能力!  
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	
	if 16 = 11
		add_sys_msg_start
		add_sys_msg 成功诱出阿萨曼！守住祭坛！全力一击！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	;2分钟计时
	VAR 13 0
	set_pworld_var 12 32 @13 1
endproc

;刷灵魂流,旧
proc 802
	
	;灵魂流波次
	get_pworld_var 24 32 1 16
	add 16 1
	set_pworld_var 24 32 @16 1
	new_mon 205280 1 11362 13656 0 240 0 1 0
	new_mon 205280 1 11274 13818 0 241 0 1 0
	new_mon 205280 1 11275 13517 0 242 0 1 0
	new_mon 205280 1 18210 14852 0 250 0 1 0
	new_mon 205280 1 18310 15022 0 251 0 1 0
	new_mon 205280 1 18409 14817 0 252 0 1 0
	NEW_TMP_ITEM 1420004 14802 14172 0 0 0 1
	NEW_TMP_ITEM 1400004 15254 14555 0 0 0 1
	NEW_TMP_ITEM 1400101 14345 14602 0 0 0 1
	;3,4
	if 16 >= 3
		new_mon 205288 1 11230 13682 0 243 0 1 0
		new_mon 205288 1 18154 15044 0 253 0 1 0
	endif
	if 16 = 5
		new_mon 205281 1 11420 13505 0 244 0 1 0
		new_mon 205281 1 18311 14723 0 254 0 1 0
	endif
	if 16 > 5 and 16 < 9
		new_mon 205288 1 11420 13505 0 244 0 1 0
		new_mon 205288 1 18311 14723 0 254 0 1 0
	endif
	if 16 >= 9
		new_mon 205288 1 11420 13505 0 244 0 1 0
		new_mon 205282 1 18311 14723 0 254 0 1 0
	endif
	;10波
	if 16 = 10
		new_mon 205287 1 14687 15884 0 260 0 1 0
;		new_mon 205280 1 11046 13628 0 ::patrol36:: 0 1 0
;		new_mon 205280 1 15026 17338 0 ::patrol37:: 0 1 0
;		new_mon 205280 1 19099 14424 0 ::patrol38:: 0 1 0
;		
;		;一个大队长灵魂
;		new_mon 205282 1 19099 14424 0 ::patrol38:: 0 1 0
;		;删分身,出真身
;		DELETE_MON_BY_DEFID 205283 1
;		new_mon 205287 1 14851 14163 0 0 0 1 2
	endif
;	add 16 1
;	;第10波预告
;	if 16 = 10
;		add_sys_msg_start
;		add_sys_msg 灵魂大队长带领的第$@16$波信仰的灵魂将在30秒后出现
;		set_sys_msg_mode 4 0 0
;		map_msg_info
;	endif
;	var 17 @16
;	mod 16 3
;	;3,6,9波预告
;	if 16 = 0
;		add_sys_msg_start
;		add_sys_msg 灵魂小队长带领的第$@17$波信仰的灵魂将在30秒后出现
;		set_sys_msg_mode 4 0 0
;		map_msg_info	
;	endif
;
;	sub 16 1
;	mod 16 3
;	;3,6,9波
;	if 16 = 0
;		
;		;一个小队长灵魂
;		new_mon 205281 1 15026 17338 0 ::patrol38:: 0 1 0
;	endif
endproc
;boss4挂了
proc 203
	;屏蔽了
	return
	;停止灵魂流
	;灵魂流开关计数
	var 12 0
	set_pworld_var 8 32 @12 1
	;删阻挡
	delete_mon_by_defid 261039 1
	;放电影
	PLAYER_MOVIE_START 1 0
	
	new_npc 1 302121 14729 13069 218 p100spy2.mac	
endproc

;祭台满血了,新
proc 204
	;删boss
	DELETE_MON_BY_DEFID 205283 1
	;删祭台
	DELETE_MON_BY_DEFID 294043 1
	;删传送门
	DELETE_MON_BY_DEFID 333049 1
	;灵魂流开关计数
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;灵魂流波次计时,波次*间隔/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;出boss6	
;	new_mon 263007 1 14851 14163 0 0 0 1 2
	;count30,祭祀计数
	var 30 0
	get_pworld_var 80 32 1 30
	if 30 > 10
		var 30 10
	endif
	add_sys_msg_start
	add_sys_msg $@30$个祭祀者成功到达黑暗祭坛献祭，行动失败 
	set_sys_msg_mode 1 0 0
	map_msg_info
	;倒计时
	;close28,关闭倒计时
	var 28 1
	set_pworld_var 72 32 @28 1
endproc
;祭台满血了,旧
proc 205
	;删boss
	DELETE_MON_BY_DEFID 205283 1
	;删祭台
	DELETE_MON_BY_DEFID 294043 1
	;灵魂流开关计数
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;灵魂流波次计时,波次*间隔/2 = 10*30/2
	VAR 13 60
	set_pworld_var 12 32 @13 1
	;出npc
	;出祭台
	;出3个采集物开关
	;npc大祭祀—德斯顿.阿萨曼
	NEW_NPC 1 306023 14851 14163 0 pw100boss4npc.mac
	;祭台怪物
	new_mon 294043 1 14773 14014 0 0 0 1 2
	;招采集物开关
	new_mon 201534 1 15571 13459 0 0 0 1 2
	new_mon 201535 1 14039 13457 0 0 0 1 2
	new_mon 201536 1 15563 14985 0 0 0 1 2
endproc
;3阶段重置
proc 301
;	add_sys_msg_start
;	add_sys_msg 重置
;	set_sys_msg_mode 4 0 0
;	map_msg_info	
	VAR 17 0
	set_pworld_var 28 32 @17 1
	delete_mon_by_defid 201556 1
	delete_mon_by_defid 201557 1
	delete_mon_by_defid 201558 1
	;机械1刷新标记,0未触发过,1触发过
	get_pworld_var 32 32 1 18
	;机械2刷新标记,0未触发过,1触发过
	get_pworld_var 36 32 1 19
	;机械3刷新标记,0未触发过,1触发过
	get_pworld_var 40 32 1 20
	strvar x y
	if 18 > 0
		;机械1掉落位置x
		get_pworld_var 44 32 1 x
		;机械1掉落位置y
		get_pworld_var 48 32 1 y
		NEW_MON_MACHINE 1 201556 0 1 @x @y 0
	endif
	
	if 19 > 0 
		;机械2掉落位置x
		get_pworld_var 52 32 1 x
		;机械2掉落位置y
		get_pworld_var 56 32 1 y
		NEW_MON_MACHINE 1 201557 0 1 @x @y 0
	endif
	if 20 > 0 
		;机械3掉落位置x
		get_pworld_var 60 32 1 x
		;机械3掉落位置y
		get_pworld_var 64 32 1 y
		NEW_MON_MACHINE 1 201558 0 1 @x @y 0
	endif
	
endproc
proc 302
	NEW_NPC 1 333045 17687 5000 0 jzsdgozc.mac
endproc
;各种调试,可删除
PROC 10
	pworld_again
ENDPROC
proc 11
        OUT_PWORLD_BY_SINGLE
endproc
PROC 12
	pworld_over
ENDPROC
proc 13
	delete_mon_by_defid 294009 1
	delete_mon_by_defid 261039 1
endproc
proc 14 
	;灵魂流开关计数
	VAR 12 3
	set_pworld_var 8 32 @12 1
	;灵魂流波次计时,波次*间隔/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
endproc
proc 15
	;10计时数
	var 10 0
	var 11 0
	var 12 0
	var 13 0
	get_pworld_var 0 32 1 10
	;地图技能开关
	get_pworld_var 4 32 1 11
	;灵魂流开关计数
	get_pworld_var 8 32 1 12
	;灵魂流波次计时,波次*间隔/2 = 10*30/2
	get_pworld_var 12 32 1 13
	add_sys_msg_start
	add_sys_msg 计时数$@10$ 地图技能开关$@11$ 灵魂流开关计数$@12$ 灵魂流波次计时$@13$ 
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc
proc 16
	;给祭台加血
	;祭坛的实例id
	get_pworld_var 16 32 1 14
	mon_set_hp 20000 @14 1
endproc
proc 17
	;count30,祭祀计数
	var 30 0
	get_pworld_var 80 32 1 30
	add 30 1
	set_pworld_var 80 32 @30 1
	delete_mon_by_defid 261059 1
	delete_mon_by_defid 261060 1
	delete_mon_by_defid 261061 1
	delete_mon_by_defid 261062 1
	delete_mon_by_defid 261063 1
	delete_mon_by_defid 261064 1
	delete_mon_by_defid 261065 1
	delete_mon_by_defid 261066 1
	delete_mon_by_defid 261067 1
	delete_mon_by_defid 261068 1
	delete_mon_by_defid 261069 1
	if 30 < 10
		add_sys_msg_start
		add_sys_msg $@30$个祭祀者成功到达黑暗祭坛献祭！ 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	get_pworld_var 8 32 1 12
	if 30 >= 10 and 12 != 6
		;灵魂流开关计数
		var 12 6
		set_pworld_var 8 32 @12 1
	endif
	strvar id 
	var id 261059
	if 30 > 0 and 30 <= 10
		add id @30
		new_mon @id 1 14808 14228 0 0 0 1 0
	endif
	
	
endproc