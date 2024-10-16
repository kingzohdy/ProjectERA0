;远古要塞副本
;创建副本事件为Proc 1，关闭副本事件为Proc 2，副本定时事件为Proc 3 （每2秒触发一次）， 副本中怪物死亡事件为Proc 4 ;副本角色进入事件为Proc 5 副本中角色死亡事件 proc 6 副本中倒计时结束事件 Proc7

proc 1
	call 90 ;变量
	call 91	;一号BOSS以及门口刷怪
endproc

proc 3
	;副本定时事件,（每2秒触发一次）.

	;所有部件死亡刷出最终BOSS变身表现，变身结束刷出最终BOSS
	get_pworld_var 148 32 1 13
	
	strvar num1 num2 num3 num4 num5 num6 num7
	GET_MAP_MON_NUM 0 228070 0 num1 0
	GET_MAP_MON_NUM 0 228068 0 num2 0
	GET_MAP_MON_NUM 0 228066 0 num3 0
	GET_MAP_MON_NUM 0 228067 0 num4 0
	GET_MAP_MON_NUM 0 228069 0 num5 0
	GET_MAP_MON_NUM 0 228071 0 num6 0
	GET_MAP_MON_NUM 0 228235 0 num7 0
	
	if num1 < 1 and num2 < 1 and num3 < 1 and num4 < 1 and num5 < 1 and num6 < 1
		if @13 = 0
			NEW_LIFE_MON 228235 1 31143 27691 0 0 0 45 4 0 0 ;刷出BOSS变身
			add 13 1
			set_pworld_var 148 32 @13 0
		endif
		if @13 = 1
			GET_MAP_MON_NUM 0 228235 0 num7 0
			if num7 < 1
				NEW_MON_BY_DIR 228072 1 31143 27691 0 0 0 45 0 71 ;最终BOSS,达到一定条件触发刷出
				TARGET_ADD_STATUS 3 @71 1095 1 7200 2
				add 13 1
				set_pworld_var 148 32 @13 0
			endif
		endif
	endif
	
	;老4达到条件添加变身状态
	strvar ret
	get_pworld_var 184 32 1 14
	get_pworld_var 168 32 1 72

	get_id_mon_info @72 ret 0 304 0 0 0 0
	
	get_pworld_var 192 32 1 15	
	get_pworld_var 224 32 1 501
	get_pworld_var 196 32 1	199
	if @304 < 1782782 and @15 = 0 and ret = 0
		TARGET_ADD_STATUS 3 @72 1038 3 7200 1
		
		add_sys_msg_start
		add_sys_msg 虚拟体吸收了血色护盾的能量,进入狂暴状态,攻击暴击能力提升，勇士们小心了！
		set_sys_msg_mode 4 0 0
		map_msg_info
		
		add_sys_msg_start
		add_sys_msg 虚拟体吸收了血色护盾的能量,进入狂暴状态,攻击暴击能力提升，勇士们小心了！
		set_sys_msg_mode 1 0 0
		map_msg_info
		
		add 15 1
		set_pworld_var 192 32 @15 1
	endif
	
	
	

endproc



proc 4
	;怪物死亡事件

	;最终BOSS部件满足条件，死亡后重生
	strvar id1 id2 id3 id4 id5 id6
	strvar ret1 ret2 ret3 ret4 ret5 ret6

	get_pworld_var 120 32 1 60 ;左脚
	get_pworld_var 124 32 1 61 ;左爪
	get_pworld_var 128 32 1 62 ;头
	get_pworld_var 132 32 1 63 ;躯干
	get_pworld_var 136 32 1 64 ;右脚
	get_pworld_var 140 32 1 65 ;右爪
	
	get_id_mon_info @60 ret1 0 402 0 0 0 0
	get_id_mon_info @61 ret2 0 403 0 0 0 0
	get_id_mon_info @62 ret3 0 404 0 0 0 0
	get_id_mon_info @63 ret4 0 405 0 0 0 0
	get_id_mon_info @64 ret5 0 406 0 0 0 0
	get_id_mon_info @65 ret6 0 407 0 0 0 0
	
	GET_MON_INFO 0 ID1 0 0 0 0 0 0
	if ID1 = 228070
		if (ret2 = 0 and @403 > 209340) or (ret3 = 0 and @404 > 209340) or (ret4 = 0 and @405 > 209340) or (ret5 = 0 and @406 > 209340) or (ret6 = 0 and @407 > 209340)
			NEW_MON_BY_DIR 228070 1 30787 28339 0 0 0 45 0 60
			set_pworld_var 120 32 @60 1
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎掌·左#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif
	
	GET_MON_INFO 0 ID2 0 0 0 0 0 0
	if ID2 = 228068
		if (ret1 = 0 and @402 > 209340) or (ret3 = 0 and @404 > 209340) or (ret4 = 0 and @405 > 209340) or (ret5 = 0 and @406 > 209340) or (ret6 = 0 and @407 > 209340)
			NEW_MON_BY_DIR 228068 1 30557 28114 0 0 0 45 0 61
			set_pworld_var 124 32 @61 1
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎爪·左#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif
	
	GET_MON_INFO 0 ID3 0 0 0 0 0 0
	if @ID3 = 228066
		if (ret1 = 0 and @402 > 209340) or (ret2 = 0 and @403 > 209340) or (ret3 = 0 and @404 > 209340) or (ret5 = 0 and @406 > 209340) or (ret6 = 0 and @407 > 209340)
			NEW_MON_BY_DIR 228066 1 30452 27793 0 0 0 45 0 62
			set_pworld_var 128 32 @62 1 
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎头#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif
	
	GET_MON_INFO 0 ID4 0 0 0 0 0 0
	if ID4 = 228067
		if (ret1 = 0 and @402 > 209340) or (ret2 = 0 and @403 > 209340) or (ret3 = 0 and @404 > 209340) or (ret5 = 0 and @406 > 209340) or (ret6 = 0 and @407 > 209340)
			NEW_MON_BY_DIR 228067 1 30485 27457 0 0 0 45 0 63
			set_pworld_var 132 32 @63 1 
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎躯#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif
	
	GET_MON_INFO 0 ID5 0 0 0 0 0 0
	if ID5 = 228069
		if (ret1 = 0 and @402 > 209340) or (ret2 = 0 and @403 > 209340) or (ret3 = 0 and @404 > 209340) or (ret4 = 0 and @405 > 209340) or (ret6 = 0 and @407 > 209340)
			NEW_MON_BY_DIR 228069 1 30676 27115 0 0 0 45 0 64
			set_pworld_var 136 32 @64 1 
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎爪·右#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif
	
	GET_MON_INFO 0 ID6 0 0 0 0 0 0
	if ID6 = 228071
		if (ret1 = 0 and @402 > 209340) or (ret2 = 0 and @403 > 209340) or (ret3 = 0 and @404 > 209340) or (ret4 = 0 and @405 > 209340) or (ret5 = 0 and @406 > 209340)
			NEW_MON_BY_DIR 228071 1 30893 26973 0 0 0 45 0 65
			set_pworld_var 140 32 @65 1 
			add_sys_msg_start
		    add_sys_msg 存在生命值超过15%的躯干部件时，#G虎掌·右#n将再次召唤回战场
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	endif

	GET_MON_INFO 0 300 301 0 0 0 0 0
	get_pworld_var 0  32 1 10
	
	;统计门口2个激光塔是否全部死亡
	get_pworld_var 20 32 1 20
	if @301 = @20
		add 10 1
		set_pworld_var 0 32 @10 1
	endif

	get_pworld_var 32 32 1 23
	if @301 = @23
		add 10 1
		set_pworld_var 0 32 @10 1
	endif

	;如果2个激光塔全部死亡,删除第一个BOSS前面的阻挡
	if @10 = 2
		get_pworld_var 48 32 1 40
		DEL_ID_MON @40 0
		add 10 1
		set_pworld_var 0 32 @10 1			
	endif
	
	;老1超级激光塔死亡，删除血色护盾和不可被击血色棺，刷出可击的血色棺
	get_pworld_var 200 32 1 200
	get_pworld_var 220 32 1	500
	get_pworld_var 228 32 1 502
	if @301 = @502
		del_id_mon @200 0 ;删除血色护盾
		del_id_mon @500 0 ;删除不可击血色棺
		NEW_MON_BY_DIR 228061 1 31566 15053 0 0 0 0 1 2
			
		add_sys_msg_start
		add_sys_msg 超级激光塔已经被摧毁，血色棺护盾消失，打破血色棺将法老王引出来吧！
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif

	;血色棺挂掉，刷出老1
	if @300 = 228061
		NEW_MON_BY_DIR 228060 1 31566 15053 0 0 0 1 1 50
		TARGET_ADD_STATUS 3 @50 1095 1 7200 2
	endif	

	;第一个桥,桥头的超级光塔
	get_pworld_var 44 32 1 26 ;第二个超级激光
	if @301 = @26
		get_pworld_var 56 32 1 42
		DEL_ID_MON @42 0	
	endif

	;老1死亡，删掉第一个桥前面的阻挡
	if @300 = 228060
		get_pworld_var 52 32 1 41
		DEL_ID_MON  @41 0
	endif	

	;第二个桥,桥头堡
	get_pworld_var 8  32 1 12
	get_pworld_var 108 32 1 55
	if @301 = @55 ;第二个桥,桥头堡超级塔
		add 12 1
		set_pworld_var 8 32 @12 1
	endif	

	if @12 = 1
		get_pworld_var 72 32 1 46
		del_id_mon @46 0 ;删除第二个桥通往老3的阻挡
	endif		

	if @300 = 228064
		get_pworld_var 60 32 1 43
		DEL_ID_MON @43 0 ;第一个资源点死亡,删除通往老2的阻挡
	endif

	if @300 = 228065
		get_pworld_var 64 32 1 44
		DEL_ID_MON @44 0 ;老2死亡,删除通往第二个桥的阻挡，同时刷出传说NPC
		NEW_NPC 0 228042 34100 7400 135 80fbshangren.mac
	endif		

	if @300 = 228072 ;最终BOSS死亡,刷出传送门和评分面板
		NEW_NPC 1 228049 31200 27700 45 fb80out.mac 
		pworld_over
		OPEN_PWORLD_AWARD_UI
				
		add_sys_msg_start
		add_sys_msg 副本结束,请通过传送门离开!
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif		
	
	get_pworld_var 4  32 1 11
	get_pworld_var 100 32 1 53 
	if @301 = @53 
		add 11 1
		set_pworld_var 4 32 @11 1			
	endif	

	if @11 = 1
		add 11 1
		set_pworld_var 4 32 @11 1
		get_pworld_var 68 32 1 45
		del_id_mon @45 0 ;删除第二个桥上中心点阻挡				
	endif	
	
	get_pworld_var 168 32 1 72
	if @301 = @72
		get_pworld_var 76 32 1 47
		del_id_mon @47 0
	endif
endproc

;变量设定
proc 90
	var 10 0 ;老1门口激光塔死亡数量计数	
	var 11 0 ;第二个桥,桥中心的超级塔计数,超级塔全部死亡,则撤销阻挡	
	var 12 0 ;第二个桥,桥头堡上的塔计数,超级塔
	var 13 0 ;最终BOSS防止重复刷用的变量	
	var 14 0 ;记录老3血量降低次数
	var 15 0 ;老3变身提示	
	
	set_pworld_var 0 32 @10 1
	set_pworld_var 4 32 @11 1
	set_pworld_var 8 32 @12 1
	set_pworld_var 148	32	@13 1
	set_pworld_var 184	32	@14	1
	set_pworld_var 192	32	@15	1
endproc



;全部刷怪
proc 91

	NEW_MON 228081 1 31545	11318  0 0 0 0 20     		;初级激光塔
	set_pworld_var 20 32 @20 1
	NEW_MON_BY_DIR 228063 1 31810	11324	0 0 0 0	0	21     		;	换成血卫
	set_pworld_var 24 32 @21 1
	NEW_MON_BY_DIR 228063 1 32727	11374	0 0 0 0	0	22  		;	换成血卫
	set_pworld_var 28 32 @22 1
	NEW_MON 228081 1 32960	11394 0 0 0 0 23   			;初级激光塔
	set_pworld_var 32 32 @23 1
	
	;一号BOSS点,一个超级塔,4个血卫,血色棺护盾和血色棺
	NEW_MON 228051 1 31566 15053 0 0 0 1 200				;一号BOSS护盾
	TARGET_ADD_STATUS 3 @200 1033 6 7200	2
	set_pworld_var 200 32 @200 1
	
	NEW_MON_BY_DIR 228112 1 31566 15053 0 0 0 0 1 500		;不可被攻击的血色棺，血色棺死亡时刷出法老王	
	set_pworld_var 220 32 @500 1
	
	NEW_MON 228076 1 32689 15035 0 0 0 1 502         		;一号BOSS边上的超级塔	
	set_pworld_var 228 32 @502 1
	
	NEW_MON_BY_DIR	228063 1 32425 14710 0 0 0 0 1 100	;4个血卫
	SET_MON_GROUP 1 @100 100
	NEW_MON_BY_DIR	228063 1 32924 14663 0 0 0 0 1 100
	SET_MON_GROUP 1 @100 100	
	NEW_MON_BY_DIR	228063 1 33007 15341 0 0 0 0 1 100
	SET_MON_GROUP 1 @100 100
	NEW_MON_BY_DIR	228063 1 32434 15384 0 0 0 0 1 100
	SET_MON_GROUP 1 @100 100
	LINK_MON 1 100



	
	NEW_MON_BY_DIR	228074	1	14890	32660	0	0	0	45	0	72	;	虚拟体第一阶段
	set_pworld_var 168 32 @72 1	
	TARGET_ADD_STATUS 3 @72 1095 1 7200 2
	; NEW_MON_BY_DIR 228111 1	31154 27699 0 0 0 0 0 199
	; set_pworld_var 196 32 @199 1	
	
	NEW_MON_BY_DIR	228068	1	30787	28339	0	0	0	0	0	60	;	左右抓
	set_pworld_var 120 32 @60 1    
	TARGET_ADD_STATUS 3 @60 2105 1 9000 2
	SET_MON_GROUP 1 @60 210
	NEW_MON_BY_DIR	228069	1	30557	28114	0	0	0	0	0	61	;	左右抓
	set_pworld_var 124 32 @61 1  
	TARGET_ADD_STATUS 3 @61 2105 1 9000 2
	SET_MON_GROUP 1 @61 210
	NEW_MON_BY_DIR	228066	1	30452	27793	0	0	0	0	0	62	;	头
	set_pworld_var 128 32 @62 1  
	TARGET_ADD_STATUS 3 @62 2105 1 9000 2
	SET_MON_GROUP 1 @62 210
	NEW_MON_BY_DIR	228067	1	30485	27457	0	0	0	0	0	63	;	躯干
	set_pworld_var 132 32 @63 1  
	TARGET_ADD_STATUS 3 @63 2105 1 9000 2
	SET_MON_GROUP 1 @63 210
	NEW_MON_BY_DIR	228070	1	30676	27115	0	0	0	0	0	64	;	左右腿
	set_pworld_var 136 32 @64 1  
	TARGET_ADD_STATUS 3 @64 2105 1 9000 2
	SET_MON_GROUP 1 @64 210
	NEW_MON_BY_DIR	228071	1	30893	26973	0	0	0	0	0	65	;	左右腿
	set_pworld_var 140 32 @65 1  
	TARGET_ADD_STATUS 3 @65 2105 1 9000 2
	SET_MON_GROUP 1 @65 210
	LINK_MON 1 210	
	
	;第一个桥桥头部分,2个巡逻的小怪,4个小塔,一个超级塔
	NEW_MON 228076 1 26523	16186 0 0 0 0 26 ;	桥头中间超级激光塔,激光塔死亡,桥头通往主干的阻挡消失
	SET_MON_GROUP 1 @26 201
	set_pworld_var 44 32 @26 1   
	NEW_MON	228086 1 27017	15746 0 0 0 1 2	;桥头小塔光1 	换成小怪228086,228087

	NEW_MON	228086 1 26960	16690 0 0 0 1 2 ;桥头小塔冰2	换成小怪
	NEW_MON	228087 1 26025	16628 0 0 0 1 2 ;桥头小塔光3	换成小怪
	NEW_MON	228087 1 26090	15693 0 0 0 1 2 ;桥头小塔冰4	换成小怪
	NEW_MON_BY_DIR 228106 1 26799	16524 0 700	0	0 1 104 	;半机械近战巡逻1
	SET_MON_GROUP 1 @104 201
	NEW_MON_BY_DIR 228107 1 26923	16023 0 701	0	0 1 105		;半机械远程巡逻2
	SET_MON_GROUP 1 @105 201
	LINK_MON 1 201	


;第二步刷怪,第一个桥,桥头的超级塔死亡后,刷第一个桥上主干部分的怪物
	;第一个桥,主干部分的刷怪
	NEW_MON_BY_DIR 228106 1 21620	15868 0 702	0	0 1 2	;25469	16295	主干巡逻1
	NEW_MON_BY_DIR 228107 1 21768	15880 0 703 0	0 1 2	;25503	15936	主干巡逻2

	; NEW_MON	228081 1 23583	16152 0 0 0 1 2 ;	初级光塔2
	NEW_MON	228081 1 21970	16064 0 0 0 1 2 ;	初级光塔4
	NEW_MON	228083 1 23603	15830 0 0 0 1 2 ;	初级冰塔6
	; NEW_MON	228083 1 21994	15730 0 0 0 1 2 ;	初级冰塔8
	
	NEW_MON_BY_DIR	228086	1	22648	16035	0 0	0	130	1	2 
	NEW_MON_BY_DIR	228086	1	22654	15847	0 0	0	130	1	2

	NEW_MON_BY_DIR	228087	1	22870	16049	0 0	0	130	1	2
	NEW_MON_BY_DIR	228087	1	22884	15859	0 0	0	130	1	2		


;第一个桥通往资源点的怪
	NEW_MON	228081	1	20058	15157	0	0	0	1	2	;第一个桥到资源点的初级光塔
	;第一个塔旁边的四个小怪
	NEW_MON_BY_DIR	228086	1	20158	15057	0	0	0	115	1	100	;	半机械近战
	SET_MON_GROUP 1 @100 200
	NEW_MON_BY_DIR	228087	1	20058	15357	0	0	0	115	1	101	;	半机械远程
	SET_MON_GROUP 1 @101 200
	NEW_MON_BY_DIR	228086	1	20158	15257	0	0	0	115	1	102	;	半机械近战
	SET_MON_GROUP 1 @102 200
	NEW_MON_BY_DIR	228087	1	20008	15257	0	0	0	115	1	103	;	半机械远程
	SET_MON_GROUP 1 @103 200
	LINK_MON 1 200
	
	NEW_MON	228083	1	21258	13739	0	0	0	1	2	;	冰 第二个初级塔
	; NEW_MON	228081	1	20661	13055	0	0	0	1	2	;	初级光塔,路上第三个塔
	;第三个塔旁边怪物堆
	NEW_MON_BY_DIR	228086	1	20900	12989	0	0	0	90	1	104	;	半机械近战
	SET_MON_GROUP 1 @104 201
	NEW_MON_BY_DIR	228087	1	20900	12889	0	0	0	90	1	105	;	半机械远程
	SET_MON_GROUP 1 @105 201
	NEW_MON_BY_DIR	228086	1	21050	12989	0	0	0	90	1	106	;	半机械近战
	SET_MON_GROUP 1 @106 201
	NEW_MON_BY_DIR	228087	1	21050	12889	0	0	0	90	1	107	;	半机械远程
	SET_MON_GROUP 1 @107 201
	LINK_MON 1 201
	
	NEW_MON	228081	1	22064	12294	0	0	0	1	2	;	光塔...第四个
	NEW_MON	228083	1	22427	12049	0	0	0	1	2	;	冰...第五个
	;第四个和第五个塔附近怪物堆
	NEW_MON_BY_DIR	228086	1	22558	12372	0	0	0	90	1	108	;	半机械近战
	SET_MON_GROUP 1 @108 202
	NEW_MON_BY_DIR	228087	1	22558	12272	0	0	0	90	1	109	;	半机械远程
	SET_MON_GROUP 1 @109 202
	NEW_MON_BY_DIR	228086	1	22408	12372	0	0	0	90	1	110	;	半机械近战
	SET_MON_GROUP 1 @110 202
	NEW_MON_BY_DIR	228087	1	22408	12272	0	0	0	90	1	111	;	半机械远程
	SET_MON_GROUP 1 @111 202
	LINK_MON 1 202
	
	NEW_MON	228083	1	22613	10638	0	0	0	1	2	;	冰...第六个
	NEW_MON_BY_DIR	228086	1	22914	10505	0	0	0	90	1	112	;	半机械近战
	SET_MON_GROUP 1 @112 203
	NEW_MON_BY_DIR	228087	1	22914	10405	0	0	0	90	1	113	;	半机械远程
	SET_MON_GROUP 1 @113 203
	NEW_MON_BY_DIR	228086	1	22814	10505	0	0	0	90	1	114	;	半机械近战
	SET_MON_GROUP 1 @114 203
	NEW_MON_BY_DIR	228087	1	22814	10405	0	0	0	90	1	115	;	半机械远程
	SET_MON_GROUP 1 @115 203
	NEW_MON_BY_DIR	228086	1	22714	10505	0	0	0	90	1	116	;	半机械近战
	SET_MON_GROUP 1 @116 203
	NEW_MON_BY_DIR	228087	1	22714	10405	0	0	0	90	1	117	;	半机械远程
	SET_MON_GROUP 1 @117 203
	LINK_MON 1 203	
	
	NEW_MON_BY_DIR	228064	1	20204	8718	0	0	0	0	0	74	;	资源点
	TARGET_ADD_STATUS 3 @74 1095 1 7200 2       
	; NEW_MON	228078	1	19814	9161	0	0	0	1	2	;	超级冰塔
	; NEW_MON	228076	1	20640	8268	0	0	0	1	2	;	超级光塔
	NEW_MON_BY_DIR	228098	1	21570	9252	0	704	0	0	1	2	;	战车巡逻
	NEW_MON_BY_DIR	228099	1	21934	12758	0	705	0	0	1	2	;	塔克巡逻
	NEW_MON_BY_DIR	228098	1	20465	15696	0	706	0	0	1	2	;	战车巡逻	

	NEW_MON_BY_DIR	228088	1	20609	8199	0	0	0	110	1	2	;	战车1
	NEW_MON_BY_DIR	228086	1	20682	8458	0	0	0	110	1	118	;	半机械近战站岗
	SET_MON_GROUP 1 @118 204
	NEW_MON_BY_DIR	228086	1	20573	8381	0	0	0	110	1	119	;	半机械近战站岗
	SET_MON_GROUP 1 @119 204
	NEW_MON_BY_DIR	228086	1	20459	8303	0	0	0	110	1	120	;	半机械近战站岗
	SET_MON_GROUP 1 @120 204
	NEW_MON_BY_DIR	228087	1	20815	8229	0	0	0	110	1	121	;	半机械远程站岗
	SET_MON_GROUP 1 @121 204
	NEW_MON_BY_DIR	228087	1	20669	8080	0	0	0	110	1	122	;	半机械远程站岗
	SET_MON_GROUP 1 @122 204
	LINK_MON 1 204
	
	NEW_MON_BY_DIR	228089	1	19755	9218	0	0	0	110	1	123	;	战车2
	NEW_MON_BY_DIR	228086	1	20002	9204	0	0	0	110	1	124	;	半机械近战站岗
	SET_MON_GROUP 1 @123 205
	NEW_MON_BY_DIR	228086	1	19880	9078	0	0	0	110	1	125	;	半机械近战站岗
	SET_MON_GROUP 1 @124 205
	NEW_MON_BY_DIR	228086	1	19758	8953	0	0	0	110	1	126	;	半机械近战站岗
	SET_MON_GROUP 1 @125 205
	NEW_MON_BY_DIR	228087	1	19788	9479	0	0	0	110	1	127	;	半机械远程站岗
	SET_MON_GROUP 1 @126 205
	NEW_MON_BY_DIR	228087	1	19522	9295	0	0	0	110	1	128	;	半机械远程站岗
	SET_MON_GROUP 1 @126 205
	LINK_MON 1 205
	

;小机械BOSS到资源点刷怪
	
	; NEW_MON	228083	1	17574	11330	0	0	0	1	2	;	塔1	
	; NEW_MON	228081	1	18200	11400	0	0	0	1	2	;	塔2	
	NEW_MON_BY_DIR	228088	1	17795	10950	0	0	0	170	1	100	;	战车
	SET_MON_GROUP 1 @100 200
	;NEW_MON_BY_DIR	228089	1	18195	10950	0	0	0	170	1	101	;	坦克
	;SET_MON_GROUP 1 @101 200
	NEW_MON_BY_DIR	228096	1	17912	10980	0	0	0	170	1	102	;	近战
	SET_MON_GROUP 1 @102 200
	NEW_MON_BY_DIR	228096	1	17886	11146	0	0	0	170	1	103	;	近战
	SET_MON_GROUP 1 @103 200
	NEW_MON_BY_DIR	228097	1	17851	11307	0	0	0	170	1	104	;	远程
	SET_MON_GROUP 1 @104 200
	LINK_MON 1 200
	
	NEW_MON	228083	1	17815	13124	0	0	0	1	2	;	塔1	
	NEW_MON	228081	1	17250	13124	0	0	0	1	2	;	塔2	
	
	; NEW_MON	228083	1	16900	15319	0	0	0	1	2	;	塔1	
	; NEW_MON	228081	1	16268	14808	0	0	0	1	2	;	塔2	
	NEW_MON_BY_DIR	228088	1	16423	15530	0	0	0	160	1	105	;	战车
	SET_MON_GROUP 1 @105 201
	;NEW_MON_BY_DIR	228089	1	16126	15263	0	0	0	160	1	106	;	坦克
	;SET_MON_GROUP 1 @106 201
	NEW_MON_BY_DIR	228096	1	16685	15301	0	0	0	160	1	107	;	近战
	SET_MON_GROUP 1 @107 201
	NEW_MON_BY_DIR	228097	1	16528	15172	0	0	0	160	1	108	;	远程
	SET_MON_GROUP 1 @108 201
	NEW_MON_BY_DIR	228096	1	16358	15053	0	0	0	160	1	109	;	近战	
	SET_MON_GROUP 1 @109 201
	LINK_MON 1 201
	
	; NEW_MON	228083	1	13832	16695	0	0	0	1	2	;	塔1	
	; NEW_MON	228081	1	13218	17100	0	0	0	1	2	;	塔2	
	NEW_MON_BY_DIR	228088	1	13662	16987	0	0	0	175	1	202	;	战车
	SET_MON_GROUP 1 @202 202
	NEW_MON_BY_DIR	228089	1	13108	16940	0	0	0	175	1	202;	坦克
	SET_MON_GROUP 1 @202 202
	NEW_MON_BY_DIR	228096	1	13369	16946	0	0	0	175	1	202;	近战
	SET_MON_GROUP 1 @202 202
	NEW_MON_BY_DIR	228096	1	13383	16687	0	0	0	175	1	202;	近战
	SET_MON_GROUP 1 @202 202
	NEW_MON_BY_DIR	228097	1	13366	17204	0	0	0	175	1	202;	远程	
	SET_MON_GROUP 1 @202 202
	LINK_MON 1 202

	; NEW_MON	228083	1	10300	17800	0	0	0	1	2	;	塔1	
	; NEW_MON	228081	1	10300	16800	0	0	0	1	2	;	塔2	
	NEW_MON_BY_DIR	228088	1	10300	17000	0	0	0	135	1	2	;	战车
	SET_MON_GROUP 1 @2 203
	NEW_MON_BY_DIR	228089	1	10300	17600	0	0	0	135	1	2	;	坦克
	SET_MON_GROUP 1 @2 203
	NEW_MON_BY_DIR	228096	1	10600	17000	0	0	0	135	1	2	;	近战
	SET_MON_GROUP 1 @2 203
	NEW_MON_BY_DIR	228096	1	10600	17600	0	0	0	135	1	2	;	近战
	SET_MON_GROUP 1 @2 203
	NEW_MON_BY_DIR	228097	1	10600	17300	0	0	0	135	1	2	;	远程
	SET_MON_GROUP 1 @2 203	
	LINK_MON 1	203	
	
	NEW_MON_BY_DIR	228065	1	7702	18897	0	0	0	157	0	75	;	小BOSS	
	TARGET_ADD_STATUS	3	@75 1095 1 7200 2
	NEW_MON	228078	1	7360	18547	0	0	0	1	2	;	超冰	
	
	NEW_MON	228080	1	8049	19252	0	0	0	1	2	;	超炮	

	NEW_MON_BY_DIR	228088	1	7675	18205	0	0	0	157	1	2	;	坦克1	
	NEW_MON_BY_DIR	228088	1	8390	18850	0	0	0	157	1	2	;	坦克2	
	NEW_MON_BY_DIR	228089	1	8250	18400	0	0	0	157	1	2	;	战车1	
	; NEW_MON_BY_DIR	228089	1	8348	18460	0	0	0	157	1	2	;	战车2	
	NEW_MON_BY_DIR	228097	1	7800	18027	0	0	0	157	1	2	;	远程站岗机械兵	
	SET_MON_GROUP 1 @2 204
	NEW_MON_BY_DIR	228096	1	7940	18027	0	0	0	157	1	2	;	近战站岗机械兵	
	SET_MON_GROUP 1 @2 204
	NEW_MON_BY_DIR	228097	1	8080	18027	0	0	0	157	1	2	;	近战站岗机械兵	
	SET_MON_GROUP 1 @2 204
	NEW_MON_BY_DIR	228096	1	8600	18917	0	0	0	157	1	2	;	远程站岗机械兵
	SET_MON_GROUP 1 @2 204	
	NEW_MON_BY_DIR	228097	1	8600	18777	0	0	0	157	1	2	;	近战站岗机械兵
	SET_MON_GROUP 1 @2 204	
	NEW_MON_BY_DIR	228096	1	8600	18637	0	0	0	157	1	2	;	近战站岗机械兵	
	SET_MON_GROUP 1 @2 204
	LINK_MON 1	204


;第二个桥
	NEW_MON_BY_DIR	228096	1	9240	24914	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	9507	24705	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228096	1	9604	25407	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	9885	25210	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228096	1	9920	25846	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	10213	25660	0	0	0	25	1	2	;	远程站岗机器人
	; NEW_MON_BY_DIR	228096	1	9284	24973	0	0	0	25	1	2	;	近战站岗机器人
	; NEW_MON_BY_DIR	228097	1	9562	24772	0	0	0	25	1	2	;	远程站岗机器人
	; NEW_MON_BY_DIR	228096	1	9652	25475	0	0	0	25	1	2	;	近战站岗机器人
	; NEW_MON_BY_DIR	228097	1	9924	25282	0	0	0	25	1	2	;	远程站岗机器人
	; NEW_MON_BY_DIR	228096	1	9986	25936	0	0	0	25	1	2	;	近战站岗机器人
	; NEW_MON_BY_DIR	228097	1	10278	25712	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228096	1	9349	25065	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	9631	24861	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228096	1	9700	25532	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	9968	25330	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228096	1	10019	25984	0	0	0	25	1	2	;	近战站岗机器人
	NEW_MON_BY_DIR	228097	1	10308	25785	0	0	0	25	1	2	;	远程站岗机器人
	NEW_MON_BY_DIR	228099	1	10170	25901	0	707	0	25	1	2	;	坦克
	NEW_MON_BY_DIR	228094	1	10022	25818	0	708	0	25	1	2	;	近战巡逻机器人
	; NEW_MON_BY_DIR	228095	1	10084	25778	0	709	0	25	1	2	;	远程巡逻机器人
	NEW_MON_BY_DIR	228094	1	10146	25738	0	710	0	25	1	2	;	近战巡逻机器人

	;第二个桥中心
	NEW_MON_BY_DIR	228080	1	10619	26509	0	0	0	25	1	53	;	超炮,正中心位置
	SET_PWORLD_VAR 100 32 @53 1 

	NEW_MON_BY_DIR	228103	1	11868	28201	0	711 0	25	1	2	;	导弹车
	SET_MON_GROUP 1 @2 205
	NEW_MON_BY_DIR	228104	1	11705	28091	0	712 0	25	1	2	;	火箭筒机器人
	SET_MON_GROUP 1 @2 205
	NEW_MON_BY_DIR	228104	1	11754	28049	0	713 0	25	1	2	;	火箭筒机器人
	SET_MON_GROUP 1 @2 205
	NEW_MON_BY_DIR	228104	1	11802	28023	0	714 0	25	1	2	;	火箭筒机器人
	SET_MON_GROUP 1 @2 205
	LINK_MON 1	205
	
	NEW_MON	228077	1	12349	28874	0	0	0	0	55	;	中心电塔
	SET_PWORLD_VAR 108 32 @55 1 

	SET_PWORLD_VAR 116 32 @57 1 
	NEW_MON_BY_DIR	228090	1	12362	29235	0	0	0	25	1	2	;	导弹车
	NEW_MON_BY_DIR	228090	1	12689	29009	0	0	0	25	1	2	;	导弹车
	NEW_MON_BY_DIR	228093	1	12105	29035	0	0	0	25	1	2	;	火箭兵
	; NEW_MON_BY_DIR	228093	1	12222	28961	0	0	0	25	1	2	;	火箭兵
	NEW_MON_BY_DIR	228093	1	12574	28722	0	0	0	25	1	2	;	火箭兵
	; NEW_MON_BY_DIR	228093	1	12460	28799	0	0	0	25	1	2	;	火箭兵
	; NEW_MON_BY_DIR	228096	1	12115	28908	0	0	0	25	1	2	;	近战机器人
	NEW_MON_BY_DIR	228096	1	12172	28847	0	0	0	25	1	2	;	近战机器人
	NEW_MON_BY_DIR	228096	1	12393	28691	0	0	0	25	1	2	;	近战机器人
	; NEW_MON_BY_DIR	228096	1	12464	28662	0	0	0	25	1	2	;	近战机器人
	; NEW_MON_BY_DIR	228096	1	12268	28923	0	0	0	25	1	2	;	近战机器人
	; NEW_MON_BY_DIR	228096	1	12444	28811	0	0	0	25	1	2	;	近战机器人	
	
	;二号BOSS点	
	NEW_MON	228076	1	13833	33099	0	0	0	1	2	;	超光
	NEW_MON	228077	1	15228	33686	0	0	0	1	2	;	超炮
	NEW_MON	228078	1	16986	35698	0	0	0	1	2	;	超炮
	NEW_MON	228079	1	17867	33253	0	0	0	1	2	;	超炮
	NEW_MON	228080	1	11939	31041	0	0	0	1	2	;	初级光塔
	NEW_MON	228081	1	14249	29450	0	0	0	1	2	;	初级光塔
	NEW_MON	228082	1	18034	30028	0	0	0	1	2	;	超光
	NEW_MON	228083	1	12616	34894	0	0	0	1	2	;	超冰
	NEW_MON	228084	1	16168	31486	0	0	0	1	2	;	超光
	NEW_MON	228085	1	16279	32396	0	0	0	1	2	;	超冰
	
	;第三个桥.
	NEW_MON_BY_DIR	228099	1	19089	30005	0	715	0	0	1	2	;	巡逻坦克
	NEW_MON_BY_DIR	228103	1	19429	30005	0	716	0	0	1	2	;	巡逻导弹车
	NEW_MON_BY_DIR	228094	1	19089	30170	0	717	0	0	1	2	;	近战巡逻机器人
	NEW_MON_BY_DIR	228104	1	19259	30170	0	718	0	0	1	2	;	火箭兵
	NEW_MON_BY_DIR	228094	1	19429	30170	0	719	0	0	1	2	;	近战巡逻机器人
	NEW_MON_BY_DIR	228094	1	19089	29840	0	720	0	0	1	2	;	近战巡逻机器人
	NEW_MON_BY_DIR	228104	1	19259	29840	0	721	0	0	1	2	;	火箭兵
	NEW_MON_BY_DIR	228094	1	19429	29840	0	722	0	0	1	2	;	近战巡逻机器人

	NEW_MON	228085	1	22498	30225	0	0	0	1	2	;	塔位左
	NEW_MON	228085	1	22498	29790	0	0	0	1	2	;	塔位右	

	;弧形区域
	;NEW_MON_BY_DIR	228091	1	24520	32496	0	723	0	0	1	2	;	弧形区域巡逻
	NEW_MON	228079	1	23840	30001	0	0	0	1	2	;	塔,超级破甲

	NEW_MON	228081	1	22099	30178	0	0	0	1	2	;	初级激光塔
	NEW_MON	228082	1	22093	29772	0	0	0	1	2	;	初级电塔
	NEW_MON	228083	1	22801	31840	0	0	0	1	2	;	初级冰塔
	NEW_MON	228084	1	23720	31253	0	0	0	1	2	;	初级破甲塔
	NEW_MON	228085	1	22883	28133	0	0	0	1	2	;	初级炮塔
	NEW_MON	228082	1	23766	28691	0	0	0	1	2	;	初级电塔
	NEW_MON	228083	1	25049	27174	0	0	0	1	2	;	初级冰塔
	NEW_MON	228084	1	25084	27906	0	0	0	1	2	;	初级破甲塔	

	;最终BOSS点
	; NEW_MON_BY_DIR 228113 1 31154 27699 0 0 0 45 1 501	;假本体
	; set_pworld_var 224 32 @501 1
	
	
	;副本中全部的阻挡怪
	NEW_MON_BY_DIR	228105 1 32261 11619 0 0 0 0  0 40 		;一号BOSS前的阻挡
	set_pworld_var 48 32 @40 1
	NEW_MON_BY_DIR	228105 1 27912 16366 0 0 0 45 0 41  	;一号BOSS通关的阻挡，击杀BOSS该阻挡消失
	set_pworld_var 52 32 @41 1
	NEW_MON_BY_DIR	228105 1 25767 16141 0 0 0 45 0 42		;桥头堡通往桥主干阻挡,桥头的超级塔死亡则删除
	set_pworld_var 56 32 @42 1
	NEW_MON_BY_DIR	228105 1 18247	9945 0 0 0 170 0 43		;资源点通往小头领点
	set_pworld_var 60 32 @43 1	
	NEW_MON_BY_DIR	228105 1 8202	22377 0 0 0 0 0 44		;小头领点通往第二个桥
	set_pworld_var 64 32 @44 1
	NEW_MON_BY_DIR	228105 1 10927	26926 0 0 0 20 0 45		;第二个桥上中心阻挡
	set_pworld_var 68 32 @45 1
	NEW_MON_BY_DIR	228105 1 12695	29374 0 0 0 25 0 46		;第二个桥终点阻挡
	set_pworld_var 72 32 @46 1	
	NEW_MON_BY_DIR	228105 1 18683	30000 0 0 0 45 0 47		;第二号BOSS通往第三个BOSS的阻挡
	set_pworld_var 76 32 @47 1
	
	
	endproc

	



	


	
