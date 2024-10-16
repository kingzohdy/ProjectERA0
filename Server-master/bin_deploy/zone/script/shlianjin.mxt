

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 5 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想要学习炼金术吗？我就是你要找的人，往下看…
	add_talk #b#b难得有年轻人来找我这个老太婆，仿佛让我回到了18岁的萌动！喔咯咯咯咯…
	add_talk #c0040FF采油车获取途径：
	add_talk #c0040FF1. 公会城市—杰尔夫处使用公会贡献度购买
	add_talk #c0040FF2. 初级采集场—采集官员“雨飞”处使用绑银购买
	add_talk #b#b#c0040FF●初级采集场进入时间：每天16：00-23：00
	add_talk #b#b#c0040FF●“大马丁”处环组任务奖励初级采集许可证
	if ret = -1 
		add_talk #b#b
		add_talk #c186506人物达到30级后，可在此学习【炼金】专业#n
	elseif ret = 0 and LV <= 10
		add_talk #b#b
		add_talk #c186506晋升【熟练】炼金师要求：
		add_talk 1.炼金等级达到10级
		add_talk 2.炼金熟练度达到上限#n
	elseif ret = 0 and LV > 10 and LV <= 20
		add_talk #b#b
		add_talk #c186506晋升【资深】炼金师要求：
		add_talk 1.炼金等级达到20级
		add_talk 2.炼金熟练度达到上限
		add_talk 3.人物等级达到70级#n
	else
	endif

	task_add_option 4036 1 103	
	task_add_option 4037 1 104	
	task_add_option 4038 1 105	
	task_add_option 4039 1 106	
	task_add_option 4040 1 107

;	task_add_option 4022 1 303

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #炼金科技生产专业列表
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #成为见习炼金师
		endif
		
		option 300 部分炼金图纸获得方式
;	endif	

	talk
endproc 


proc 202
	OPEN_UI 41
endproc

PROC 103	
	task_do_option 4036 1
ENDPROC

PROC 104	
	task_do_option 4037 1
ENDPROC

PROC 105	
	task_do_option 4038 1
ENDPROC

PROC 106	
	task_do_option 4039 1
ENDPROC

PROC 107	
	task_do_option 4040 1
ENDPROC
	
;PROC 303	
;	task_do_option 4022 1
;ENDPROC

PROC 110
	task_stat 4020 1 10
	if 10 != 3
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b你必须达到30级后找博学士大马丁，通过他的提问测试。
		talk
		return 		   
	endif

	strvar m_Ret m_Lv
	GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 41
	else
		strvar num
		;生活技能数量
		var num 0

		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		;数量不能超过2个
		if num >= 3
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b你能学习的科技生产专业已达到上限，要学习该专业请遗忘至少一项已学的专业。
			talk
			return 
		endif
		choose 1 201 1 是否确定学习炼金科技生产专业？		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 5
endproc
proc 300
	dialog 1
	add_talk #c0040FF鉴定仪公式获得方式:#n
	add_talk #b#b#c186506★70级以下鉴定仪：65-70级野外怪物掉落#n
	add_talk #b#b#c186506★80级以下鉴定仪：公会城市内杰尔夫购买#n
	add_talk #c0040FF首饰镀金公式:#n
	add_talk #b#b#c186506★“失落之域”所有怪物一定几率掉落#n
	add_talk #b#b
	
		option 1 返回
	talk
endproc





