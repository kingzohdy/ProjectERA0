

proc 1
	DIALOG 1
	add_talk #b#b我这里有时间城最好的防具！不信？你拿旁边羊头怪雷蒙德的武器来砍我的盔甲试试！砍坏了你赔我一件，砍不坏你买一件！怎么样，敢赌一把吗。
	add_talk #b#b#c186506出售各职业的防具#n
	task_add_option 1091 1 101
	task_add_option 1092 1 102
	task_add_option 2013 1 105


;50级



	task_add_option 1029 1 108
	task_add_option 1030 1 109



	OPTION 103 #(买卖)打开商店

	task_stat 4488 1 20
	if 20 = 2
		option 200 #向防具商人巴兹尔赠送一个粽子
	endif
	talk
endproc 


PROC 101
	task_do_option 1091 1
ENDPROC
PROC 102
	task_do_option 1092 1
ENDPROC
PROC 103
	OPEN_NPC_SHOP
ENDPROC

PROC 105 
	task_do_option 2013 1
ENDPROC

PROC 108
	task_do_option 1029 1
ENDPROC
PROC 109
	task_do_option 1030 1
ENDPROC

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
	task_stat 4488 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4488 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4488 1
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



proc 2
	get_trig_task 1 2 
	if 2 = 1092	
		task_fini 1092 1
	endif
	if 2 = 2013	
		task_fini 2013 1
	endif
	if 2 = 2228	
		task_fini 2228 1
	endif
	if 2 = 1030	
		task_fini 1030 1
	endif

endproc
