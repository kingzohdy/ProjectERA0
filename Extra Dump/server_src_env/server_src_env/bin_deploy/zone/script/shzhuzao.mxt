

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 4 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我从事铸造一生，如果你只认为铸造是单纯的四肢劳作那就错了。
	add_talk #b#b没事请别打搅我，我正在思索着如何更有效的减轻装备重量。
	add_talk #c0040FF采矿车获取途径：
	add_talk #c0040FF1. 公会城市—杰尔夫处使用公会贡献度购买
	add_talk #c0040FF2. 初级采集场—采集官员“雨飞”处使用绑银购买
	add_talk #b#b#c0040FF●初级采集场进入时间：每天16：00-23：00
	add_talk #b#b#c0040FF●“大马丁”处环组任务奖励初级采集许可证
	if ret = -1 
		add_talk #b#b
		add_talk #c186506人物达到30级后，可在此学习【铸造】专业#n
	elseif ret = 0 and LV <= 10
		add_talk #b#b
		add_talk #c186506晋升【熟练】铸造师要求：
		add_talk 1.铸造等级达到10级
		add_talk 2.铸造熟练度达到上限#n
	elseif ret = 0 and LV > 10 and LV <= 20
		add_talk #b#b
		add_talk #c186506晋升【资深】铸造师要求：
		add_talk 1.铸造等级达到20级
		add_talk 2.铸造熟练度达到上限
		add_talk 3.人物等级达到70级#n
	else
	endif
;	task_add_option 3405 1 103
	
	;五一活动
	task_stat 4430 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	task_add_option 4046 1 104
	task_add_option 4047 1 105
	task_add_option 4048 1 106
	task_add_option 4049 1 107
	task_add_option 4050 1 108

	task_add_option 3405 1 305

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #铸造科技生产专业列表
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
		if m_Ret != 0	
			OPTION 110 #成为见习铸造师
		endif
		option 300 部分铸造图纸获得方式
;	endif	

	talk
endproc 

proc 202
	OPEN_UI 40
endproc

PROC 103
	task_do_option 3405 1
ENDPROC

PROC 104	
	task_do_option 4046 1
ENDPROC

PROC 105	
	task_do_option 4047 1
ENDPROC

PROC 106	
	task_do_option 4048 1
ENDPROC

PROC 107	
	task_do_option 4049 1
ENDPROC

PROC 108	
	task_do_option 4050 1
ENDPROC
	
PROC 305	
	task_do_option 3405 1
ENDPROC

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
	GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 40
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
		;数量不能超过3个
		if num >= 3
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b你能学习的科技生产专业已达到上限，要学习该专业请遗忘至少一项已学的专业。
			talk
			return 
		endif
		choose 1 201 1 是否确定学习铸造科技生产专业？		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 4
endproc

proc 300
	DIALOG 1
	add_talk #c0040FF首饰图纸获得方式:#n
	add_talk #b#b#c186506★时间城亨利、埃玛、沙丽娜处的配方匣开出#n
	add_talk #b#b#c186506★野外精英怪物有一定几率掉落铸造图纸#n
	add_talk #b#b
		
		
		option 1 返回
	
	talk
endproc 




;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4430 1
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b五月阳光金灿灿，祝福你好运连连；
			add_talk #b#b五月花开香飘飘，祝福你和和美美；
			add_talk #b#b五月喜鹊叫喳喳，祝福你平平安安；
			add_talk #b#b五月有幸福多多，祝福你快快乐乐！
			option 511 #没问题
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“礼品盒”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc



