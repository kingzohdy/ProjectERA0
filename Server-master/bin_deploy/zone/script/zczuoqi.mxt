

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我是时间城的坐骑商人，不要叫我马夫！我能卖给你任何你想要的坐骑，当然，跑车很贵！
	
	
	task_add_option 3137 1 104
	task_add_option 3136 1 105
	
	
	task_add_option 3141 1 106
	task_add_option 3145 1 107
	task_add_option 3150 1 108
	task_add_option 3155 1 109
	task_add_option 3160 1 111
	task_add_option 3138 1 112
	
;	OPTION 110 #(买卖)打开商店
	option 180 #我想进行坐骑等级转移

	task_stat 4490 1 20
	if 20 = 2
		option 200 #向坐骑大师阿诺德赠送一个粽子
	endif
	
	task_stat 3030 1 20
	if 20 = 2
		option 170 与阿诺德交谈
	endif
	talk
endproc 

PROC 104
	task_do_option 3137 1
ENDPROC
PROC 105
	task_do_option 3136 1
ENDPROC
PROC 106
	task_do_option 3141 1
ENDPROC
PROC 107
	task_do_option 3145 1
ENDPROC
PROC 108
	task_do_option 3150 1
ENDPROC
PROC 109
	task_do_option 3155 1
ENDPROC
PROC 111
	task_do_option 3160 1
ENDPROC
PROC 112
	task_do_option 3138 1
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
	task_stat 4490 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4490 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4490 1
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



PROC 110
	OPEN_NPC_SHOP
ENDPROC

proc 170
	DIALOG 1
   	ADD_TALK $_npc$       
	ADD_TALK #b#b啊，朋友，你觉得我的拓拓如何？是不是比你双腿跑得快很多？
	ADD_TALK #b#b只要你愿意帮助我，我就会送你一匹坐骑，虽然跟拓拓没的比，但至少比你走的快。放心，绝对不比别人的差！
		OPTION 171 #乐意之至 
	TALK	

endproc
proc 171
	task_fini 3030 1
endproc  

proc 2
	get_trig_task 1 2
	if 2 = 1096
		task_fini 1096 1
	endif 
	if 2 = 3138
		task_fini 3138 1
	endif

	if 2 = 3141
		task_fini 3141 1
	endif
endproc

proc 180
	OPEN_UI 74
	;打开坐骑等级转移界面
endproc 





