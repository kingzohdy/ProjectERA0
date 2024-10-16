;;;;;60副本 普通


proc 1 

	strvar menmid1 menmid2 menmid3 menmid4
	;阻挡1
	NEW_MON_BY_DIR 201063 1 16900 29200 0 0 0 30 1 menmid1 
	var 10 @menmid1
	set_pworld_var 0 32 @10 1
	
	;阻挡2
	NEW_MON_BY_DIR 201063 1 14200 22300 0 0 0 45 1 menmid2
	var 11 @menmid2
	set_pworld_var 4 32 @11 1
	
	
	;阻挡3
	NEW_MON_BY_DIR 201063 1 27600 21100 0 0 0 90 1 menmid3 
	var 12 @menmid3
	set_pworld_var 8 32 @12 1
	;阻挡大门
	NEW_MON_BY_DIR 201104 1 24800 32200 0 0 0 40 1 menmid4
	var 13 @menmid4
	set_pworld_var 12 32 @13 1
	
	
	;2号进入战斗后刷 阻挡
;	NEW_MON_BY_DIR 201063 1 8900 26000 0 0 0 45 1 menmid4
;	var 13 @menmid4
;	set_pworld_var 16 32 @13 1
	;3号进入战斗后刷 阻挡
;	NEW_MON_BY_DIR 201063 1 23800 24800 0 0 0 45 1 menmid4
;	var 13 @menmid4
;	set_pworld_var 20 32 @13 1
	

	
	
	;第一结界BOSS驭兽 npc   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar npcmid momid
	NEW_NPC 1 201102 19100 29700 110 npcxxoo.mac
	GET_MAP_NPC_ID 201102 npcmid 1
	;记录冒泡npc的实例
	var 14 @npcmid
	set_pworld_var 20 32 @14 1
	
	;第二结界烈焰结界BOSS德鲁伊;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	NEW_MON_BY_DIR 201118 1 11400 24000 0 0 0 30 1 momid	
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
	
	;第三结界牧树结界;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	NEW_MON_BY_DIR 201127 1 26200 23100 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
	SET_MON_GROUP 1 @momid 304
	NEW_MON_BY_DIR 201128 1 26100 23600 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
	SET_MON_GROUP 1 @momid 304
	LINK_MON 1 304
	
	
	;第四古武结界;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	NEW_MON_BY_DIR 201129 1 26600 8300 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
	
	
	;去专家BOSS的阻挡门;;;;;;;;;;;;;;普通副本才有+++++++++++++++++++++++++++++-------------------------------------------------
	NEW_MON_BY_DIR 201063 1 26100 15200 0 0 0 60 1 momid
	
;	声明变量
	var 50 0
	set_pworld_var 24 32 @50 1
	
;	声明变量-8字节锁;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	var 41 0
	set_pworld_var 101 8 @41 1
	var 42 0
	set_pworld_var 102 8 @42 1
	
	;普通还是专家标记---普通
	var 43 0
	set_pworld_var 103 8 @43 1

;   计时器1 用来隔30秒后 第一结界开始刷怪
	var 44 0
	set_pworld_var 104 8 @44 1
;	计时器2 刷带路径的怪
	var 45 0
	set_pworld_var 105 8 @45 1
	
;	两只牧树人都死
	var 46 0
	set_pworld_var 106 8 @46 1
	
	;专家副本倒计时标题;;;;;;;;;;;;;+++++++++++++++++++++++++++++++++++++++++++++++++++
	strvar time
	;记录创建副本时时间变量
	var time 0
	set_pworld_var 16 32 @time 0

	;刷路径怪；；；；；；；；；；；；；；
	
	call 790
	NEW_MON_BY_DIR 201152 1 15400 28100 0 519 0 30 1 Ret1 
	NEW_MON_BY_DIR 201152 1 9500 30200 0 520 0 30 1 Ret1 
	NEW_MON_BY_DIR 201152 1 19000 21000 0 521 0 30 1 Ret1 
	NEW_MON_BY_DIR 201152 1 24500 24700 0 522 0 30 1 Ret1 
	NEW_MON_BY_DIR 201152 1 28200 17800 0 523 0 30 1 Ret1 
	NEW_MON_BY_DIR 201152 1 25100 12800 0 524 0 30 1 Ret1 
	
	
	;;;;;;;;;;;;;;刷小怪;;;;;;;;;;;;;;;;;;;;;;;;;
	;1~2
	call 80
	call 81
	call 82
	call 83
	call 84
	
	;;;;;;;;;;;;刷小怪;;;;;;;;;;;;;;;;;;
	;2~3
	call 85
	call 86
;	call 87
	call 88
	call 89
;	call 90
	call 91
	;call 92

	;;;;;;;;;;;刷小怪;;;;;;;;;;;;;;;;
	;3~4
	call 93
	call 94
	call 95
	call 96
	call 97
	;call 98
	call 99
	;;;;;;;;;;;;;;;;;刷单独小怪
	NEW_MON_BY_DIR 201153 1 9500 29400 0 0 0 120 1 Ret1 	
;	NEW_MON_BY_DIR 201153 1 9500 30600 0 0 0 120 1 Ret1 	
	NEW_MON_BY_DIR 201158 1 22500 24300 0 0 0 30 1 Ret1 	
	NEW_MON_BY_DIR 201158 1 22600 24900 0 0 0 30 1 Ret1 	
	
endproc  
proc 2
;关闭副本事件

	
endproc



proc 3
;2秒触发一次


;;;;;;;;;;;;;;;;;1111111111111111111111111111111111111111111111111111111111111111
;;;;第一个区域
	strvar roc
	get_pworld_var 101 8 1 roc
;	add_sys_msg_start
;	add_sys_msg $@roc$
;	set_sys_msg_mode 1 0 0
;	map_msg_info
	if roc = 1
		;boss开始说话及刷怪
		get_pworld_var 104 8 1 44
		add 44 1
		set_pworld_var 104 8 @44 1
		if 44 = 1
			effect 0 0 playEffct(25100,32300,340,"effect\\entity\\9570_16.ent")
		endif
		if 44 = 3
			effect 0 0 playEffct(25100,32300,340,"effect\\entity\\9570_16.ent")
		endif
		;30秒后开始刷新第一波怪物----嘟嘟鸟
		if 44 = 5
			add_sys_msg_start
			add_sys_msg 驭兽者：这片结界由我掌控，踏入者非我族类，必须死！
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 44 = 10
			add_sys_msg_start
			add_sys_msg 驭兽者：一群蠢货，竟敢这么堂而皇之的站在我面前，我会让你们痛苦的死去
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 44 = 15
			add_sys_msg_start
			add_sys_msg 驭兽者：听我的号令，去干掉这些不知进退的东西
			set_sys_msg_mode 4 0 0
			map_msg_info

;			get_pworld_var 20 32 1 14
;			MON_SPEECH @14 1 2 1 嘟嘟鸟来了。。。。。。。。。。。。。。。。
			
			strvar aa
			rand aa 2
			if aa = 0
				NEW_MON_BY_DIR 201136 1 18500 30200 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79
				
				NEW_MON_BY_DIR 201136 1 18400 29700 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
			
				
				NEW_MON_BY_DIR 201136 1 18600 29200 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
				
				
				NEW_MON_BY_DIR 201136 1 19200 29000 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
				
				
			endif
			if aa = 1
				NEW_MON_BY_DIR 201136 1 18400 30000 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79  
				
				NEW_MON_BY_DIR 201136 1 18400 29400 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
				
				NEW_MON_BY_DIR 201136 1 18800 29000 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
				
				NEW_MON_BY_DIR 201136 1 19500 29100 0 0 0 120 1 50
				;SET_MON_GROUP 1 @50 79 
				
			endif
			;LINK_MON 1 79
			;计时器归零
			var 44 0
			set_pworld_var 104 8 @44 1
			;;
			var roc 0
			set_pworld_var 101 8 @roc 1
		endif
	endif
	if roc = 2
		;小怪全部死亡，刷BOSS
		get_pworld_var 20 32 1 14
	
		
		GET_MON_NUM_BY_DEFID 201075 num
		if num = 0
			get_pworld_var 104 8 1 44
			if 44 = 0
				MON_SPEECH @14 1 2 1 你们杀了我的猛兽，我会让你们付出代价
			endif
			add 44 1
			set_pworld_var 104 8 @44 1
			if 44 = 15
		
				DEL_NPC 201102 1
				NEW_MON_BY_DIR 201110 1 19200 29800 0 0 0 30 1 momid
				TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 
				var roc 0
				set_pworld_var 101 8 @roc 1
			endif
		endif
	endif

;	get_pworld_var 104 8 1 44
;	add_sys_msg_start
;	add_sys_msg roc = $@roc$   44=$@44$ 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
	;后续刷怪2~10波	；；；；；；；；；；；；；；；；；；；
	strvar num	
	if roc >= 50
		get_pworld_var 20 32 1 14 
		;获得冒泡NPC 实例
		
		if roc = 50
		;驯鹿
			GET_MON_NUM_BY_DEFID 201136 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 呦！还不错嘛，软脚虾们，不过你们高兴的太早了，尝尝被鹿角顶的滋味吧！
				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 800
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 51
		;蝎子
			GET_MON_NUM_BY_DEFID 201141 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 看样子，我确实低估了你们，接下来，让蝎子们陪你们玩玩吧。
				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 801
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 52
		;野猪
			GET_MON_NUM_BY_DEFID 201140 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 蝎子的毒刺都奈何不了你们，看样子是我小看了你们，那么再尝尝和野猪共处的乐趣吧！
				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 802
					var roc 0
					set_pworld_var 101 8 @roc 1
					
				endif
			endif
		endif
		if roc = 53
		;豹子
			GET_MON_NUM_BY_DEFID 201139 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 你们会为你们的行为付出代价的，就让豹子来招待你们吧

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 803
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 54
		;老虎
			GET_MON_NUM_BY_DEFID 201138 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 老虎，干掉他们！

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 804
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 55
		;剑齿虎
			GET_MON_NUM_BY_DEFID 201137 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 杀死我的老虎，后果就是死

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 805
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 56
		;蝙蝠
			GET_MON_NUM_BY_DEFID 201142 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 隐藏的蝙蝠们，新鲜的血液就在你们面前

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 806
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		
		endif
		if roc = 57
		;蜘蛛
			GET_MON_NUM_BY_DEFID 201143 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 黑暗中，最神秘的杀手，蜘蛛们，终结他们吧！

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 807
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		if roc = 58
		;狼
			GET_MON_NUM_BY_DEFID 201144 num
			if num = 0
				get_pworld_var 104 8 1 44
				if 44 = 0
					MON_SPEECH @14 13 2 1 苍狼们，去撕碎他们！

				endif
				add 44 1
				set_pworld_var 104 8 @44 1
				if 44 = 2
					call 808
					var roc 0
					set_pworld_var 101 8 @roc 1
				endif
			endif
		endif
		
	endif
	
endproc 


proc 4
;怪物死亡事件
endproc

proc 5
;副本角色进入事件

endproc 
proc 6
;副本中角色死亡事件

endproc
proc 7
;倒计时结束事件
endproc 

proc 80
	;第一个点
	strvar rad
	rand rad 3
	if rad = 0
		call 100
	endif
	if rad = 1
		call 101
	endif
	if rad = 2
		call 102
	endif
;	if rad = 3
;		call 103
;	endif
endproc 
proc 81
	;第二个点
	strvar rad
	rand rad 3
	if rad = 0
		call 150
	endif
	if rad = 1
		call 151
	endif
	if rad = 2
		call 152
	endif
;	if rad = 3
;		call 153
;	endif
;	if rad = 4
;		call 154
;	endif
endproc 
proc 82
	;第三个点
	strvar rad
	rand rad 3
	if rad = 0
		call 155
	endif
	if rad = 1
		call 156
	endif
	if rad = 2
		call 157
	endif
;	if rad = 3
;		call 158
;	endif
;	if rad = 4
;		call 159
;	endif
endproc 
proc 83
	;第四个点
	strvar rad
	rand rad 3
	if rad = 0
		call 160
	endif
	if rad = 1
		call 161
	endif
	if rad = 2
		call 162
	endif
;	if rad = 3
;		call 163
;	endif
endproc
proc 84
	;第五个点
	strvar rad
	rand rad 3
	if rad = 0
		call 164
	endif
	if rad = 1
		call 165
	endif
	if rad = 2
		call 166
	endif
;	if rad = 3
;		call 167
;	endif
;	if rad = 4
;		call 168
;	endif
endproc 
;;;;;;;;;;;2~3路上；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；
proc 85
	strvar rad
	rand rad 3
	if rad = 0
		call 169
	endif
	if rad = 1
		call 170
	endif
	if rad = 2
		call 171
	endif
endproc
proc 86
	strvar rad
	rand rad 3
	if rad = 0
		call 172
	endif
	if rad = 1
		call 173
	endif
	if rad = 2
		call 174
	endif
endproc
;proc 87
;	strvar rad
;	rand rad 3
;	if rad = 0
;		call 175
;	endif
;	if rad = 1
;		call 176
;	endif
;	if rad = 2
;		call 177
;	endif
;endproc
proc 88
	strvar rad
	rand rad 3
	if rad = 0
		call 178
	endif
	if rad = 1
		call 179
	endif
	if rad = 2
		call 180
	endif
endproc
proc 89
	strvar rad
	rand rad 3
	if rad = 0
		call 181
	endif
	if rad = 1
		call 182
	endif
	if rad = 2
		call 183
	endif
endproc
;proc 90
;	strvar rad
;	rand rad 3
;	if rad = 0
;		call 184
;	endif
;	if rad = 1
;		call 185
;	endif
;	if rad = 2
;		call 186
;	endif
;endproc
proc 91
	strvar rad
	rand rad 3
	if rad = 0
		call 187
	endif
	if rad = 1
		call 188
	endif
	if rad = 2
		call 189
	endif
endproc
;proc 92
;	strvar rad
;	rand rad 3
;	if rad = 0
;		call 190
;	endif
;	if rad = 1
;		call 191
;	endif
;	if rad = 2
;		call 192
;	endif
;endproc
;3~4路上；；；；；；；；；；；；；；；
proc 93
	strvar rad
	rand rad 4
	if rad = 0
		call 193
	endif
	if rad = 1
		call 194
	endif
	if rad = 2
		call 195
	endif
	if rad = 3
		call 196
	endif
endproc
proc 94
	strvar rad
	rand rad 4
	if rad = 0
		call 197
	endif
	if rad = 1
		call 198
	endif
	if rad = 2
		call 199
	endif
	if rad = 3
		call 200
	endif
endproc
proc 95
	strvar rad
	rand rad 4
	if rad = 0
		call 201
	endif
	if rad = 1
		call 202
	endif
	if rad = 2
		call 203
	endif
	if rad = 3
		call 204
	endif
endproc
proc 96
	strvar rad
	rand rad 3
	if rad = 0
		call 205
	endif
	if rad = 1
		call 206
	endif
	if rad = 2
		call 207
	endif
endproc

proc 97
	strvar rad
	rand rad 3
	if rad = 0
		call 208
	endif
	if rad = 1
		call 209
	endif
	if rad = 2
		call 210
	endif
endproc
proc 98
	strvar rad
	rand rad 3
	if rad = 0
		call 211
	endif
	if rad = 1
		call 212
	endif
	if rad = 2
		call 213
	endif
endproc
proc 99
	strvar rad
	rand rad 3
	if rad = 0
		call 214
	endif
	if rad = 1
		call 215
	endif
	if rad = 2
		call 216
	endif
endproc 

proc 100
	;1~2路上小怪 第1个点 
	strvar mondef Ret1
	var mondef 201147

	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 49
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 49
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 49
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 49
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 49
	endif
	;LINK_MON 1 49
endproc 
proc 101
;1
	strvar ad rad Ret1
	var mondef 201149

	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 50
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 50
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 50
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 50
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 50
	endif
	;LINK_MON 1 50
endproc 
proc 102
;1
	strvar ad rad Ret1
	var mondef 201151

	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 51
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 51
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 51
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 51
	endif
	NEW_MON_BY_DIR @mondef 1 13900 27800 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 51
	endif
	;LINK_MON 1 51
endproc 
;proc 103
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201079 1 13900 27800 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 52
;	endif
;	NEW_MON_BY_DIR 201079 1 13900 27800 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 52
;	endif
;	NEW_MON_BY_DIR 201079 1 13900 27800 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 52
;	endif
;	NEW_MON_BY_DIR 201079 1 13900 27800 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 52
;	endif
;	NEW_MON_BY_DIR 201079 1 13900 27800 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 52
;	endif
;	;LINK_MON 1 52
;endproc
;;;;;;第二个点;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 150
	;11
	strvar ad rad Ret1
	var mondef 201147

	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 90
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 90
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 90
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 90
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 90
	endif
	;LINK_MON 1 90
endproc
proc 151
	;11
	strvar ad rad Ret1
	var mondef 201149

	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 91
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 91
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 91
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 91
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 91
	endif
	;LINK_MON 1 91
endproc

proc 152
	;11
	strvar ad rad Ret1
	var mondef 201151

	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 93
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 93
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 93
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 93
	endif
	NEW_MON_BY_DIR @mondef 1 13500 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 93
	endif
	;LINK_MON 1 93
endproc
;proc 153
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201083 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 94
;	endif
;	NEW_MON_BY_DIR 201083 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 94
;	endif
;	NEW_MON_BY_DIR 201083 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 94
;	endif
;	NEW_MON_BY_DIR 201083 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 94
;	endif
;	NEW_MON_BY_DIR 201083 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 94
;	endif
;	;LINK_MON 1 94
;endproc
;proc 154
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201084 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 95
;	endif
;	NEW_MON_BY_DIR 201084 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 95
;	endif
;	NEW_MON_BY_DIR 201084 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 95
;	endif
;	NEW_MON_BY_DIR 201084 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 95
;	endif
;	NEW_MON_BY_DIR 201084 1 13500 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 95
;	endif
;	;LINK_MON 1 95
;endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;第3个点；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；；

proc 155
	;11
	strvar ad rad Ret1
	var mondef 201147

	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 96
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 96
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 96
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 96
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 96
	endif
	;LINK_MON 1 96
endproc
proc 156
	;11
	strvar ad rad Ret1
	var mondef 201151

	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 97
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 97
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 97
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 97
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 97
	endif
	;LINK_MON 1 97
endproc

proc 157
	;11
	strvar ad rad Ret1
	var mondef 201149

	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 98
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 98
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 98
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 98
	endif
	NEW_MON_BY_DIR @mondef 1 11500 29600 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 98
	endif
	;LINK_MON 1 98
endproc
;proc 158
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201083 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201083 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201083 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201083 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201083 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	;LINK_MON 1 94
;endproc
;proc 159
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201084 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201084 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201084 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201084 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	NEW_MON_BY_DIR 201084 1 11500 29600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 99
;	endif
;	;LINK_MON 1 99
;endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;第四个点；；；；；；；；；；；；；；
proc 160
	;
	strvar ad rad Ret1
	var mondef 201147

	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 100
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 100
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 100
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 100
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 100
	endif
	;LINK_MON 1 100
endproc 
proc 161
;1
	strvar ad rad Ret1
	var mondef 201149

	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 101
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 101
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 101
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 101
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 101
	endif
	;LINK_MON 1 101
endproc 
proc 162
;1
	strvar ad rad Ret1
	var mondef 201151

	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 102
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 102
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 102
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 102
	endif
	NEW_MON_BY_DIR @mondef 1 10400 30500 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 102
	endif
	;LINK_MON 1 102
endproc 
;proc 163
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201079 1 10400 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 103
;	endif
;	NEW_MON_BY_DIR 201079 1 10400 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 103
;	endif
;	NEW_MON_BY_DIR 201079 1 10400 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 103
;	endif
;	NEW_MON_BY_DIR 201079 1 10400 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 103
;	endif
;	NEW_MON_BY_DIR 201079 1 10400 30500 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 103
;	endif
;	;LINK_MON 1 103
;endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;第五个点；；；；；；；；；

proc 164
	;11
	strvar ad rad Ret1
	var mondef 201147

	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 104
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 104
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 104
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 104
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 104
	endif
	;LINK_MON 1 104
endproc
proc 165
	;11
	strvar ad rad Ret1
	var mondef 201149

	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 105
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 105
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 105
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 105
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 105
	endif
	;LINK_MON 1 105
endproc

proc 166
	;11
	strvar ad rad Ret1
	var mondef 201151

	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 106
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 106
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 106
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 106
	endif
	NEW_MON_BY_DIR @mondef 1 8500 26000 100 0 0 30 1 Ret1 
	if 1 = 0
		;SET_MON_GROUP 1 @Ret1 106
	endif
	;LINK_MON 1 106
endproc
;proc 167
;	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201083 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 107
;	endif
;	NEW_MON_BY_DIR 201083 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 107
;	endif
;	NEW_MON_BY_DIR 201083 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 107
;	endif
;	NEW_MON_BY_DIR 201083 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 107
;	endif
;	NEW_MON_BY_DIR 201083 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 107
;	endif
;	;LINK_MON 1 107
;endproc
;proc 168
	;11
;	strvar ad rad Ret1
;	NEW_MON_BY_DIR 201084 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 108
;	endif
;	NEW_MON_BY_DIR 201084 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 108
;	endif
;	NEW_MON_BY_DIR 201084 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 108
;	endif
;	NEW_MON_BY_DIR 201084 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 108
;	endif
;	NEW_MON_BY_DIR 201084 1 8200 25600 100 0 0 30 1 Ret1 
;	if 1 = 0
;		;SET_MON_GROUP 1 @Ret1 108
;	endif
;	;LINK_MON 1 108
;endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;2~3号之间
proc 169
; 第1个点 
	strvar Ret1
	var mondef 201155

		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 109
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 109
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 109
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 109
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 109
		endif
	;LINK_MON 1 109
endproc 
proc 170
; 第1个点 
	strvar Ret1
	var mondef 201156

		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 110
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 110
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 110
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 110
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 110
		endif
	;LINK_MON 1 110
endproc 
proc 171
; 第1个点 
	strvar Ret1
	var mondef 201159

		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 111
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 111
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 111
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 111
		endif
		NEW_MON_BY_DIR @mondef 1 19500 20700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 111
		endif
	;LINK_MON 1 111
endproc 
;第二个点
proc 172
	strvar Ret1
	var mondef 201155

		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 113
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 113
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 113
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 113
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 113
		endif
	;LINK_MON 1 113
endproc 
proc 173
	strvar Ret1
	var mondef 201156

		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 114
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 114
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 114
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 114
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 114
		endif
	;LINK_MON 1 114
endproc 
proc 174
	strvar Ret1
	var mondef 201159

		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 112
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 112
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 112
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 112
		endif
		NEW_MON_BY_DIR @mondef 1 20300 23000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 112
		endif
	;LINK_MON 1 112
endproc 
;第三个点
proc 175
	strvar Ret1
	var mondef 201155

		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 115
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 115
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 115
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 115
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 115
		endif
	;LINK_MON 1 115
endproc 
proc 176
	strvar Ret1
	var mondef 201156

		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 116
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 116
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 116
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 116
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 116
		endif
	;LINK_MON 1 116
endproc 
proc 177
	strvar Ret1
	var mondef 201159

		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 117
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 117
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 117
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 117
		endif
		NEW_MON_BY_DIR @mondef 1 21500 25100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 117
		endif
	;LINK_MON 1 117
endproc 
;第四个点
proc 178
	strvar Ret1
		var mondef 201155

		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 118
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 118
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 118
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 118
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 118
		endif
	;LINK_MON 1 118
endproc 
proc 179
	strvar Ret1
		var mondef 201156

		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 119
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 119
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 119
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 119
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 119
		endif
	;LINK_MON 1 119
endproc 
proc 180
	strvar Ret1
	var @mondef 201159

		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 120
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 120
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 120
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 120
		endif
		NEW_MON_BY_DIR @mondef 1 25900 25000 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 120
		endif
	;LINK_MON 1 120
endproc 
;第五个点
proc 181
	strvar Ret1
	var mondef 201155

		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 121
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 121
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 121
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 121
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 121
		endif
	;LINK_MON 1 121
endproc 
proc 182
	strvar Ret1
	var mondef 201156

		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 122
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 122
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 122
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 122
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 122
		endif
	;LINK_MON 1 122
endproc 
proc 183
	strvar Ret1
	var mondef 201159

		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 123
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 123
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 123
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 123
		endif
		NEW_MON_BY_DIR @mondef 1 27700 24500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 123
		endif
	;LINK_MON 1 123
endproc 

;第六个点
;proc 184
;	strvar Ret1
;		NEW_MON_BY_DIR 201088 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 124
;		endif
;		NEW_MON_BY_DIR 201088 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 124
;		endif
;		NEW_MON_BY_DIR 201088 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 124
;		endif
;		NEW_MON_BY_DIR 201088 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 124
;		endif
;		NEW_MON_BY_DIR 201088 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 124
;		endif
;	;LINK_MON 1 124
;endproc 
;proc 185
;	strvar Ret1
;		NEW_MON_BY_DIR 201089 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 125
;		endif
;		NEW_MON_BY_DIR 201089 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 125
;		endif
;		NEW_MON_BY_DIR 201089 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 125
;		endif
;		NEW_MON_BY_DIR 201089 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 125
;		endif
;		NEW_MON_BY_DIR 201089 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 125
;		endif
;
;	;LINK_MON 1 125
;endproc 
;proc 186
;	strvar Ret1
;		NEW_MON_BY_DIR 201090 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 126
;		endif
;		NEW_MON_BY_DIR 201090 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 126
;		endif
;		NEW_MON_BY_DIR 201090 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 126
;		endif
;		NEW_MON_BY_DIR 201090 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 126
;		endif
;		NEW_MON_BY_DIR 201090 1 24200 22800 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 126
;		endif
;
;	;LINK_MON 1 126
;endproc 

;第七个点
proc 187
	strvar Ret1
	var mondef 201155

		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 127
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 127
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 127
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 127
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 127
		endif
	;LINK_MON 1 127
endproc 
proc 188
	strvar Ret1
	var mondef 201156

		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 128
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 128
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 128
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 128
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 128
		endif

	;LINK_MON 1 128
endproc 
proc 189
	strvar Ret1
	var mondef 201159

		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 129
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 129
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 129
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 129
		endif
		NEW_MON_BY_DIR @mondef 1 24200 22800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 129
		endif

	;LINK_MON 1 129
endproc 
;第八个点
;proc 190
;	strvar Ret1
;		NEW_MON_BY_DIR 201091 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 130
;		endif
;		NEW_MON_BY_DIR 201091 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 130
;		endif
;		NEW_MON_BY_DIR 201091 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 130
;		endif
;		NEW_MON_BY_DIR 201091 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 130
;		endif
;		NEW_MON_BY_DIR 201091 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 130
;		endif
;	;LINK_MON 1 130
;endproc 
;proc 191
;	strvar Ret1
;		NEW_MON_BY_DIR 201092 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 131
;		endif
;		NEW_MON_BY_DIR 201092 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 131
;		endif
;		NEW_MON_BY_DIR 201092 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 131
;		endif
;		NEW_MON_BY_DIR 201092 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 131
;		endif
;		NEW_MON_BY_DIR 201092 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 131
;		endif
;
;	;LINK_MON 1 131
;endproc 
;proc 192
;	strvar Ret1
;		NEW_MON_BY_DIR 201093 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 132
;		endif
;		NEW_MON_BY_DIR 201093 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 132
;		endif
;		NEW_MON_BY_DIR 201093 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 132
;		endif
;		NEW_MON_BY_DIR 201093 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 132
;		endif
;		NEW_MON_BY_DIR 201093 1 25600 21600 100 0 0 30 1 Ret1 
;		if 1 = 0
;			;SET_MON_GROUP 1 @Ret1 132
;		endif
;
;	;LINK_MON 1 132
;endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;3~4路上小怪
proc 193
	strvar Ret1
	var mondef 201164
 
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 133
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 133
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 133
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 133
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 133
		endif
	;LINK_MON 1 133
endproc 
proc 194
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 134
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 134
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 134
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 134
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 134
		endif
	;LINK_MON 1 134
endproc  
proc 195
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 135
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 135
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 135
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 135
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 135
		endif
	;LINK_MON 1 135
endproc   
proc 196
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 136
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 136
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 136
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 136
		endif
		NEW_MON_BY_DIR @mondef 1 25600 21600 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 136
		endif
	;LINK_MON 1 136
endproc 
;第二个点
proc 197
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 137
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 137
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 137
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 137
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 137
		endif
	;LINK_MON 1 137
endproc 
proc 198
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 138
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 138
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 138
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 138
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 138
		endif
	;LINK_MON 1 138
endproc 
proc 199
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 139
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 139
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 139
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 139
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 139
		endif
	;LINK_MON 1 139
endproc 
proc 200
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 140
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 140
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 140
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 140
		endif
		NEW_MON_BY_DIR @mondef 1 28800 17500 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 140
		endif
	;LINK_MON 1 140
endproc 
;第三个点
proc 201
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 141
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 141
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 141
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 141
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 141
		endif
	;LINK_MON 1 141
endproc 
proc 202
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 142
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 142
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 142
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 142
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 142
		endif
	;LINK_MON 1 142
endproc 
proc 203
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 143
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 143
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 143
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 143
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 143
		endif
	;LINK_MON 1 143
endproc 
proc 204
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 144
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 144
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 144
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 144
		endif
		NEW_MON_BY_DIR @mondef 1 24500 13200 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 144
		endif
	;LINK_MON 1 144
endproc 
;第四个点
proc 205
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 145
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 145
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 145
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 145
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 145
		endif
	;LINK_MON 1 145
endproc 
proc 206
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 146
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 146
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 146
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 146
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 146
		endif
	;LINK_MON 1 146
endproc 
proc 207
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 147
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 147
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 147
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 147
		endif
		NEW_MON_BY_DIR @mondef 1 24500 9700 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 147
		endif
	;LINK_MON 1 147
endproc 
;第五个点
proc 208
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 148
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 148
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 148
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 148
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 148
		endif
	;LINK_MON 1 148
endproc 
proc 209
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 149
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 149
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 149
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 149
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 149
		endif
	;LINK_MON 1 149
endproc 
proc 210
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 150
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 150
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 150
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 150
		endif
		NEW_MON_BY_DIR @mondef 1 26700 10800 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 150
		endif
	;LINK_MON 1 150
endproc 
;第六个点
proc 211
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 151
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 151
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 151
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 151
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 151
		endif
	;LINK_MON 1 151
endproc 
proc 212
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 152
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 152
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 152
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 152
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 152
		endif
	;LINK_MON 1 152
endproc 
proc 213
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 153
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 153
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 153
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 153
		endif
		NEW_MON_BY_DIR @mondef 1 28500 10100 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 153
		endif
	;LINK_MON 1 153
endproc 

;;;;;新增一拨
proc 214
	strvar Ret1
	var mondef 201164

		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 154
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 154
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 154
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 154
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 154
		endif
		;LINK_MON 1 154
endproc 
proc 215
	strvar Ret1
	var mondef 201165

		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 155
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 155
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 155
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 155
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 155
		endif
		;LINK_MON 1 155
endproc 
proc 216
	strvar Ret1
	var mondef 201168

		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 156
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 156
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 156
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 156
		endif
		NEW_MON_BY_DIR @mondef 1 27200 18900 100 0 0 30 1 Ret1 
		if 1 = 0
			;SET_MON_GROUP 1 @Ret1 156
		endif
		;LINK_MON 1 156
endproc 

;;;;;;;;;;;;;;;;;;;;;;;带路径的怪物

proc 790
	NEW_MON_BY_DIR 201149 1 13400 22700 0 511 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 300
	NEW_MON_BY_DIR 201149 1 13800 22700 0 512 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 300
	;LINK_MON 1 300
	
	NEW_MON_BY_DIR 201146 1 12800 25600 0 513 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 301
	NEW_MON_BY_DIR 201146 1 12800 26200 0 514 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 301
	;LINK_MON 1 301
	
	NEW_MON_BY_DIR 201151 1 9900 25100 0 515 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 302
	NEW_MON_BY_DIR 201151 1 9400 25100 0 516 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 302
	;LINK_MON 1 302
	
;	NEW_MON_BY_DIR 201147 1 8200 30500 0 517 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 303
	
;	NEW_MON_BY_DIR 201147 1 8700 30500 0 518 0 30 1 Ret1 
	;SET_MON_GROUP 1 @Ret1 303

	;LINK_MON 1 303
endproc


proc 800
;驯鹿
	strvar a b mondef
	var mondef 201141

	rand a 2
	if a = 0
		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83 
		NEW_MON_BY_DIR @mondef 1 18900 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		rand b 4
		if b = 0
			NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		if b = 1
			NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		if b = 2
			NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		if b = 3
			NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		
	endif
	if a = 1
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		rand b 4
		if b = 0
			NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		if b = 1
			NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
		if b = 2
			NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83 
		endif
		if b = 3
			NEW_MON_BY_DIR @mondef 1 18900 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 83
		endif
	endif
	;LINK_MON 1 83
endproc
proc 801
;蝎子
	strvar a 
	var mondef 201140

	rand a 2
	if a = 0
		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		
		rand b 2
		if b = 0
			NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
			NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
		endif
		if b = 1
			NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
			NEW_MON_BY_DIR @mondef 1 18900 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
		endif
	
	endif
	if a = 1
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		NEW_MON_BY_DIR @mondef 1 18900 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 80
		rand b 2
		if b = 0
			NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
			NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
		endif
		if b = 1
			NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
			NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 80
		endif
	endif
	;LINK_MON 1 80
endproc 
proc 802
;野猪
	strvar a 
	var mondef 201139

	rand a 8
		if a = 0 or a = 1 or a = 2 or a = 3 or a = 4 or a = 5 or a = 6
			NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87
		endif
		if a = 1 or a = 2 or a = 3 or a = 4 or a = 5 or a = 6 or a = 7
			NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87
		endif
		if a = 2 or a = 3 or a = 4 or a = 5 or a = 6 or a = 7 or a = 0
			NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87 
		endif
		if a = 3 or a = 4 or a = 5 or a = 6 or a = 7 or a = 0 or a = 1
			NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87 
		endif	
		if a = 4 or a = 5 or a = 6 or a = 7 or a = 0 or a = 1 or a = 2
			NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87 
		endif
		if a = 5 or a = 6 or a = 7 or a = 0 or a = 1 or a = 2 or a = 3
			NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87 
		endif
		if a = 6 or a = 7 or a = 0 or a = 1 or a = 2 or a = 3 or a = 4
			NEW_MON_BY_DIR @mondef 1 18900 29000 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87 
		endif
		if a = 7 or a = 0 or a = 1 or a = 2 or a = 3 or a = 4 or a = 5
			NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
			;SET_MON_GROUP 1 @50 87
		endif
	;LINK_MON 1 87
endproc 
proc 803
;豹子
	var mondef 201138

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86 
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86 
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86 
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86 
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 86
		;LINK_MON 1 86
	
endproc 
proc 804
;老虎
	var mondef 201137

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85 
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 85
		;LINK_MON 1 85
endproc 
proc 805
;剑齿虎
	var mondef 201142

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84 
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84 
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 84
		;LINK_MON 1 84
endproc 
proc 806
;蝙蝠
	var mondef 201143

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 81
		;LINK_MON 1 81		
endproc 
proc 807
;蜘蛛
	var mondef 201144

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82 
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82 
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82 
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 82
		;LINK_MON 1 82
endproc 
proc 808
;狼
	var mondef 201145

		NEW_MON_BY_DIR @mondef 1 18500 30200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18400 30000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18400 29700 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83 
		NEW_MON_BY_DIR @mondef 1 18400 29400 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18600 29200 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 18800 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 19200 29000 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		NEW_MON_BY_DIR @mondef 1 19500 29100 0 0 0 121 1 50
		;SET_MON_GROUP 1 @50 83
		;LINK_MON 1 83
endproc 
	








