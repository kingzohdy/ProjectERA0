

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b虽然曾拜师克尔泽与泰戈门下，但我从未放弃对装备打造新领域的探索。
	add_talk #b#b经过多年钻研，我除了找到了装备解封技术外，不仅发现装备拥有直接进阶为更高档次，还摸索到了装备融合的力量。
	add_talk #b#b
	add_talk #c186506装备进阶：将50级以上的卓越装备进阶成史诗套装

;	add_talk #b#b
;	add_talk #b#b#c186506可进行[装备进阶]
;	add_talk #b#b可进行[装备解封]
	
	strvar s1
	task_stat 2258 1 s1
	if s1 = 2 or s1 = 0
		add_talk #c186506装备融合：赋予高于60级且卓越品质以上的装备融合属性（完成“融合力量!”任务[61级]后解锁）#n
	else
		add_talk #c186506装备融合：赋予高于60级且卓越品质以上的装备融合属性#n
	endif

	TASK_ADD_OPTION 2258 0 150
	
	option 120 #装备进阶
	option 100 #装备解封
	
	if s1 = 3
		option 300 #装备融合
	endif

	task_stat 4495 1 20
	if 20 = 2
		option 200 #向装备进阶大师惠杰丰赠送一个粽子
	endif
	
	talk
endproc 

proc 100
	open_ui 57
endproc
proc 120
	open_ui 76
endproc 

proc 150
	TASK_DO_OPTION 2258 0
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
	task_stat 4495 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4495 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4495 1
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
proc 300
	open_ui 87
endproc
proc 502
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 


