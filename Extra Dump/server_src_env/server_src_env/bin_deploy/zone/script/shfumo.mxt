

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 6 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我就是附魔大师，有事吗天行者。
	if ret = -1 
		add_talk #b#b
		add_talk #c186506人物达到30级后，可在此学习【附魔】专业#n
	elseif ret = 0 and LV <= 10
		add_talk #b#b
		add_talk #c186506晋升【熟练】附魔师要求：
		add_talk 1.附魔等级达到10级
		add_talk 2.附魔熟练度达到上限#n
	elseif ret = 0 and LV > 10 and LV <= 20
		add_talk #b#b
		add_talk #c186506晋升【资深】附魔师要求：
		add_talk 1.附魔等级达到20级
		add_talk 2.附魔熟练度达到上限
		add_talk 3.人物等级达到70级#n
	else
	endif
	
	task_add_option 4011 1 101
	task_add_option 4012 1 102

	task_add_option 4026 1 103
	task_add_option 4027 1 104
	task_add_option 4028 1 105
	task_add_option 4029 1 106
	task_add_option 4030 1 107

	;task_add_option 4020 1 301


;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #附魔科技生产专业列表
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #成为见习附魔师
		endif
		
		
		option 300 部分附魔图纸获得方式
		
;	endif	

	talk
endproc 

proc 202
	OPEN_UI 42
endproc

PROC 103	
	task_do_option 4026 1
ENDPROC

PROC 104	
	task_do_option 4027 1
ENDPROC

PROC 105	
	task_do_option 4028 1
ENDPROC

PROC 106	
	task_do_option 4029 1
ENDPROC

PROC 107	
	task_do_option 4030 1
ENDPROC

;PROC 301	
;	task_do_option 4020 1
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
	GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 42
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
		choose 1 201 1 是否确定学习附魔科技生产专业？		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 6
endproc

PROC 101
	task_do_option 4011 1
ENDPROC
PROC 102
	task_do_option 4012 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4011
		task_fini 4011 1
	endif
	if 2 = 4012
		task_fini 4012 1
	endif

endproc
proc 300
	DIALOG 1
	add_talk #c0040FF装备附魔配方获得方式:#n
	add_talk #b#b#c186506★时间城亨利、埃玛、沙丽娜处的配方匣开出#n
	add_talk #b#b#c186506★野外精英怪物有一定几率掉落附魔配方#n
	add_talk #b#b

		option 1 返回
	talk
endproc 

