

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#Race#，你找我有什么事，我可是很忙的。
	add_talk #b#b别以为我小个子就欺负我，我可是采药大师，天天上百个学生等着我教导呢。
	add_talk #b#b#c186506人物达到35级可在此学习[采药]科技生产专业#n

	task_add_option 4041 1 103
	task_add_option 4042 1 104
	task_add_option 4043 1 105
	task_add_option 4044 1 106
	task_add_option 4045 1 107

;	task_add_option 4023 1 304
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #采药科技生产专业列表
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #成为见习采药师
		endif
;	endif
	
		option 111 #前往寻宝秘境采集草药
;	endif	

	strvar RetTsk
	task_stat 3123 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	

	talk
endproc 

proc 202
	OPEN_UI 38
endproc

PROC 103	
	task_do_option 4041 1
ENDPROC

PROC 104	
	task_do_option 4042 1
ENDPROC

PROC 105	
	task_do_option 4043 1
ENDPROC

PROC 106	
	task_do_option 4044 1
ENDPROC

PROC 107	
	task_do_option 4045 1
ENDPROC
	
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b哇呼！我又有圣诞礼物了！谢你啦！$_player$！
		option 210 #交出圣诞礼物
	talk
endproc 
proc 210
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3123 1
		endif
	endif
endproc 
	
	
	
	
;PROC 304	
;	task_do_option 4023 1
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
	GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
	if m_Ret = 0	
		OPEN_UI 38
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
		choose 1 201 1 是否确定学习采药科技生产专业？		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 2
endproc

proc 111
	transmit 4401 11000 13200 1
endproc 



