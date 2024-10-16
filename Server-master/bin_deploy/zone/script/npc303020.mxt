proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你好，有何贵干。
	add_talk #b#b这里除了提供把附加属性转移给另一件装备的【#c186506装备属性转移#n】服务。
	add_talk #b#b还有提高装备附加属性值的【#c186506重铸附加属性#n】服务。
	; add_talk #b#b#c186506转移装备属性可把附加属性转给另一件装备。#n
	; add_talk #b#b#c186506重铸附加属性可提高装备的附加属性值。#n
	add_talk #b#b拥有扎实的基础，才能站在更高的顶端。
	; add_talk #b#b【#c186506装备属性转移#n】【#c186506附加属性重铸#n】可让你的装备更胜一筹，造出属于你独一无二的极品装备。
	; add_talk #b#b拥有扎实的基础，才能站在更高的顶端。
	
	task_stat 2259 1 10
	task_stat 3174 1 30
	if 10 = 2 or 10 = 0
		add_talk #b#b
		add_talk #c186506装备属性转移-完成“改造大师的需求”任务[56级]#n
	endif
	if 30 = 2 or 30 = 0
		add_talk #b#b
		add_talk #c186506附加属性重铸-完成“斯迪芬奇的考验”任务[65级]#n
	endif
	TASK_ADD_OPTION 2259 0 150
	TASK_ADD_OPTION 3174 0 160
	
	if 10 = 3
		option 101 #【装备属性转移】 
	endif
	if 30 = 3
		option 102 #【重铸附加属性】
	endif
	
	task_stat 4494 1 20
	if 20 = 2
		option 200 #向装备改造师斯迪芬奇赠送一个粽子
	endif
	talk
	
	
endproc 

proc 101
	DIALOG 1
	add_talk ★转移作用★
	add_talk #b#b可将装备的【#cff4e00附加属性#n】转移给目标装备。
	; add_talk #b#b
	add_talk ★转移说明★
	add_talk #b#b转移后提供属性转移的装备将破碎消失。
	add_talk #b#b#c186506只转移绿色的附加属性，不转移其它属性。#n
	; add_talk #b#b
	; add_talk ★转移保障★
	; add_talk #b#b保留目标装备的【#cff4e00强化星级#n】
	; add_talk #b#b保留目标装备的【#cff4e00基础属性#n】
	; add_talk #b#b保留目标装备的【#cff4e00洗炼属性#n】
	; add_talk #b#b保留目标装备的【#cff4e00附魔属性#n】
	; add_talk #b#b保留目标装备的【#cff4e00镶嵌宝石#n】
	
		option 100 #低阶转移（同品质装备间转移）
		option 250 #高阶转移（跨品质装备间转移）
		option 300 #转移石、提炼器获得方式
	
	talk
	
endproc

proc 102
	DIALOG 1
	add_talk ★重铸作用★
	add_talk #b#b可提升装备【#cff4e00附加属性#n】【#cff4e00洗炼属性#n】的属性值。
	add_talk ★重铸说明★
	add_talk #b#b可单独选择每条属性进行重铸。
	add_talk #b#b#c186506只重铸所选附加属性的值，不重铸属性类型。#n
	
		option 110 #重铸附加属性（只重铸属性值）
	
	talk
	
endproc

proc 100
	open_ui 79
endproc

proc 110
	open_ui 80
endproc

proc 150
	TASK_DO_OPTION 2259 0
endproc

proc 160
	TASK_DO_OPTION 3174 0
endproc

proc 250
	open_ui 85
endproc 
proc 300
	DIALOG 1
	add_talk ★转移石★
	add_talk #b#b向时间城流浪汉捐献有几率获得#n
	add_talk #b#b“失落之域三层”马基尔掉落#n
	add_talk #b#b
	add_talk ★绿晶提炼器★
	add_talk #b#b65级“龙蛋谷-合作”任务奖励#n
	add_talk ★蓝晶提炼器★
	add_talk #b#b“失落之域三层”马基尔掉落#n

	
		option 1 返回

		
	talk
endproc 
 
proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4494 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4494 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4494 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 
