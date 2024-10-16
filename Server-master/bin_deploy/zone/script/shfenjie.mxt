

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b噢！你好，亲爱的朋友，我是驻时间城的分解大师，请问有何贵干？
	add_talk #b#b#c186506人物达到35级可在此学习[分解]科技生产专业#n

	task_add_option 4031 1 103	
	task_add_option 4032 1 104	
	task_add_option 4033 1 105	
	task_add_option 4034 1 106	
	task_add_option 4035 1 107

;	task_add_option 4021 1 302

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #分解科技生产专业列表
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #成为见习分解师
		endif
;	endif	


	talk
endproc 


proc 202
	OPEN_UI 39
endproc

PROC 103	
	task_do_option 4031 1
ENDPROC
	
PROC 104	
	task_do_option 4032 1
ENDPROC
	
PROC 105	
	task_do_option 4033 1
ENDPROC
	
PROC 106	
	task_do_option 4034 1
ENDPROC
	
PROC 107	
	task_do_option 4035 1
ENDPROC

;PROC 302	
;	task_do_option 4021 1
;ENDPROC

PROC 110
	task_stat 4020 1 10
	if 10 != 3
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b你必须达到35级后找博学士大马丁，通过他的提问测试。
		talk
		return 		   
	endif
	
	strvar m_Ret m_Lv
	GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 39
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
		if num >= 2
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b你能学习的科技生产专业已达到上限，要学习该专业请遗忘至少一项已学的专业。
			talk
			return 
		endif
		
		choose 1 201 1 是否确定学习分解科技生产专业？		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 3
endproc




