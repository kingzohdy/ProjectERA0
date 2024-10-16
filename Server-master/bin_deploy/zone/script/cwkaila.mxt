;凯拉


proc 1


	dialog 1
	add_talk $_npc$:
	add_talk #b#b有时候，我们为了生存不得不伤害动物，但若是能驯服它们，让它们与我们处于同一战线，岂不是两全齐美吗？

	task_add_option 3002 1 101	
	task_add_option 3003 1 102	

	task_add_option 3007 1 106	
	
	TASK_STAT 3003 1 20
	if 20 = 2
	  	option 170 开始测试
;		option 201 #跳过测试
		option 273 掷骰子碰碰运气吧
		
	endif

	talk
endproc

proc 101
	task_do_option 3002 1

endproc
proc 102
	task_do_option 3003 1

endproc

proc 106
	task_do_option 3007 1

endproc
proc 2
	get_trig_task 1 2
	if 2 = 3007
		TASK_FINI 3007 0
	endif
	if 2 = 3003
		dialog 1
		add_talk $_npc$:
		add_talk #b#b初入彩虹岛，不知道该选什么宠物吗？想知道最适合您的宠物是谁吗？
		add_talk #b#b通过下面的测试，可以根据您的性格，推荐最适合您的宠物哦！测试内容包括测试您的性格和游戏倾向，要根据您的第一感觉选哦！
		TASK_STAT 3003 1 30
		if 30 = 2
			option 170 开始测试
;			option 201 #跳过测试
			option 273 掷骰子碰碰运气吧
		ENDIF
		talk
	
	endif
	if 2 = 3004
		
	endif
	
endproc 
proc 201 
	return
endproc 

proc 170

	get_pworld_var 8 32 1 30
	if @30 != 0
		del_npc @30 0
	endif
	get_pworld_var 12 32 1 31
	if @31 != 0
		DELETE_MON_BY_DEFID @31 0
	endif
	
	get_pworld_var 16 32 1 32
	if @32 != 0
		DELETE_MON_BY_DEFID @32 0
	endif

	get_task_var 3003 0 8 1 12
	var 12 0
	set_task_var 3003 0 8 @12 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b你的星座是？
		
		option 200 白羊座（3.12-4.19）
		option 200 金牛座（4.20-5.20）
		option 200 双子座（5.21-6.21）
		option 200 巨蟹座（6.22-7.22）
		option 200 狮子座（7.23-8.23）
		option 200 处女座（8.23-9.22）
		option 200 天秤座（9.23-10.23）
		option 200 天蝎座（10.24-11.22）
		option 200 射手座（11.23-12.21）
		option 200 摩羯座（12.22-1.19）
		option 200 水瓶座（1.20-2.18）
		option 200 双鱼座（2.19-3.20）
	talk
	
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b在草丛里发现有一个蛋，你的第一想法是？
		
		option 210 把他孵化出来
		option 211 把它煮来尝尝
	talk
endproc

proc 210
	get_task_var 3003 0 8 1 12
	var 12 1
	set_task_var 3003 0 8 @12 1

	;认真
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喜欢受到他人的关心么？
		
		option 220 我要万众瞩目
		option 221 众人予我皆浮云
	talk
	
endproc 
proc 211
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喜欢受到他人的关心么？
		
		option 220 我要万众瞩目
		option 221 众人予我皆浮云
	talk
	
endproc 
proc 220
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 2
		set_task_var 3003 0 8 @12 1
	endif

	;英勇
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喜欢一个人行动？
		
		option 230 英雄总是孤独的
		option 231 不要让我孤苦伶仃
	talk

endproc 
proc 221
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喜欢一个人行动？
		
		option 230 英雄总是孤独的
		option 231 不要让我孤苦伶仃
	talk
endproc 

proc 230
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 3
		set_task_var 3003 0 8 @12 1
	endif

	;冷静
	dialog 1
	add_talk $_npc$:
	add_talk #b#b遇到困难时你解决问题的方法是？
		
		option 240 武力迅速解决
		option 241 耐心寻找方法
	talk
	
endproc 
proc 231 
	dialog 1
	add_talk $_npc$:
	add_talk #b#b遇到困难时你解决问题的方法是？
		
		option 240 武力迅速解决
		option 241 耐心寻找方法
	talk
	
endproc

proc 240
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 4
		set_task_var 3003 0 8 @12 1
	endif

	;鲁莽
	dialog 1
	add_talk $_npc$:
	add_talk #b#b很容易与他人成为朋友？
		
		option 250 我的朋友满天下
		option 251 人生得一知己足矣
	talk
	
endproc
proc 241

	dialog 1
	add_talk $_npc$:
	add_talk #b#b很容易与他人成为朋友？
		
		option 250 我的朋友满天下
		option 251 人生得一知己足矣
	talk
	
endproc

proc 250

	dialog 1
	add_talk $_npc$:
	add_talk #b#b如果上天赋予一种力量，你希望是？
		
		option 260 天生神力
		option 261 刀枪不入
	talk
endproc
proc 251
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 5
		set_task_var 3003 0 8 @12 1
	endif

	;孤僻
	dialog 1
	add_talk $_npc$:
	add_talk #b#b如果上天赋予一种力量，你希望是？
		
		option 260 天生神力
		option 261 刀枪不入
	talk
endproc
proc 260
	get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 6
		set_task_var 3003 0 8 @12 1
	endif	

	;热血
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你的回答很不错，我想想应该给你什么宠物……
		
		option 270 继续
	talk
endproc
proc 261
		get_task_var 3003 0 8 1 12
	if 12 = 0
		var 12 7
		set_task_var 3003 0 8 @12 1
	endif

	;胆小
		dialog 1
	add_talk $_npc$:
	add_talk #b#b你的回答很不错，让我想想应该给你什么宠物……
		
		option 270 继续
	talk
endproc
proc 270
	dialog 1
	add_talk $_npc$

	get_task_var 3003 0 8 1 12

	switch 12
		case 1
		;认真
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：耀耀

	;		add_talk #b#b认真的松鼠猎手最擅长提高命中和闪避
		endcase
		case 2

		;鲁莽
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：东东

	;		add_talk #b#b鲁莽的松鼠猎手最擅长提高暴击率3%	
			
		endcase
		case 3
		;热血
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：涛涛

	;		add_talk #b#b热血的松鼠猎手最擅长提高物攻和法攻5%

		endcase
		case 4
		;胆小
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：南南

	;		add_talk #b#b胆小的松鼠猎手最擅长提高物防和法防5%
		endcase
		case 5
		;冷静
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：泽泽

	;		add_talk #b#b冷静的松鼠猎手最擅长提高生命上限10%	

		endcase
		case 6
		;英勇
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：鹏鹏

	;		add_talk #b#b英勇的松鼠猎手最擅长提高替身率5%
		endcase
		case 7
		;孤僻
			add_talk #b#b经过一系列测试，最适合你的伙伴是：松鼠枪手：佳佳

	;		add_talk #b#b孤僻的松鼠猎手最擅长提高降低替身率5%
		endcase
	endswitch
		option 271 #就是它了
		option 170 再测一次
		option 273 有没有带点想象力的办法？
	talk
endproc

proc 271

	get_task_var 3003 0 8 1 12
	get_pworld_var 8 32 1 30
	switch 12
		case 1
			new_npc 0 352000 6806 5935 0 cwtiexin1.mac
			var 30 352000
			set_pworld_var 8 32 @30 1
			
			GET_MAP_NPC_ID 352000 50 0 
			mon_speech @50 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 2               
			new_npc 0 352001 6806 5935 0 cwtiexin2.mac
			var 30 352001
			set_pworld_var 8 32 @30 1
			
			GET_MAP_NPC_ID 352001 51 0 
			mon_speech @51 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 3               
			new_npc 0 352002 6806 5935 0 cwtiexin3.mac
			var 30 352002
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352002 52 0 
			mon_speech @52 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 4               
			new_npc 0 352003 6806 5935 0 cwtiexin4.mac
			var 30 352003
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352003 53 0 
			mon_speech @53 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 5               
			new_npc 0 352004 6806 5935 0 cwtiexin5.mac
			var 30 352004
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352004 54 0 
			mon_speech @54 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 6               
			new_npc 0 352005 6806 5935 0 cwtiexin6.mac
			var 30 352005
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352005 55 0 
			mon_speech @55 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase              
		case 7               
			new_npc 0 352006 6806 5935 0 cwtiexin7.mac
			var 30 352006
			set_pworld_var 8 32 @30 1
			GET_MAP_NPC_ID 352006 56 0 
			mon_speech @56 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
		endcase             
	endswitch
;   ADD_FINI_TASK 3003 500	

endproc
proc 273
	dialog 1
	add_talk $_npc$:
	add_talk #b#b啊，$_player$，对刚才的结果不满意吗？还是你也不喜欢墨守成规？不如我们来试试这个新玩意好吗？
	add_talk #b#b<凯拉将一颗八面骰子递到你手中>
	add_talk #b#b充满想象力与惊喜的人生才能够丰富多彩，对吧？转动它，看看谁才是你命中注定的伙伴！
;    ADD_FINI_TASK 3003 500	
		option 300 #看看我命中注定的宠物是谁
	talk
endproc

proc 300
		
	get_pworld_var 8 32 1 30
	if @30 != 0
		del_npc @30 0
	endif
	get_pworld_var 12 32 1 31
	if @31 != 0
		DELETE_MON_BY_DEFID @31 0
	endif
	get_pworld_var 16 32 1 32
	if @32 != 0
		DELETE_MON_BY_DEFID @32 0
	endif
	
	call 310	
		;限制玩家只能领取一个骰子
;		GET_ITEM_N UM_INPAK 5300377 0 430
;		if 430 = 0
;			ADD_ITEM_NUM_INPAK 5300377 1 1 0	
;			add_sys_msg_start
;			add_sys_msg 用这个骰子试试你的运气如何！
;			set_sys_msg_mode 3 0 0
;			msg_info
;		else
;			add_sys_msg_start
;			add_sys_msg 你身上还有个骰子没用过呢……
;			set_sys_msg_mode 3 0 0
;			msg_info
;		endif

	return
endproc
proc 310

	get_task_var 3003 1 8 1 13
	var 13 1
	set_task_var 3003 1 8 @13 1

	rand 50 8
	add 50 100
	switch 50

		case 100
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9555_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 1
			set_task_var 3003 3 8 @15 1		
			
			
		endcase
		case 101
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9556_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 2
			set_task_var 3003 3 8 @15 1	
			
		endcase
		case 102
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9557_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 3
			set_task_var 3003 3 8 @15 1	
		endcase
		case 103
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9558_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 4
			set_task_var 3003 3 8 @15 1	
		endcase
		
		case 104
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9559_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 5
			set_task_var 3003 3 8 @15 1	
		endcase
		case 105
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9560_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 6
			set_task_var 3003 3 8 @15 1	
		endcase
		case 106
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9561_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 7
			set_task_var 3003 3 8 @15 1	
		endcase
		case 107
			effect 0 0 playEffct(6700,5300,"effect\\entity\\9562_16.ent")
			get_task_var 3003 3 8 1 15
			var 15 8
			set_task_var 3003 3 8 @15 1	
		endcase

	endswitch


endproc 
 

























