
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b一整天都在闲晃你都不会觉得无聊吗？要不要跟朋友进游乐场玩玩呢？
	add_talk #b#b#c186506欢乐嘉年华开放时间：
	add_talk #b#b#c0181f9 13:00-13:20，18:00-18:20
	
	task_add_option 3331 1 100
	
	OPTION 202 欢乐嘉年华
	
	strvar RetTsk
	task_stat 3124 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	talk
	
endproc

PROC 100
	task_do_option 3331 1
ENDPROC

proc 202

	dialog 1
	add_talk $_npc$:
	add_talk #b#b里面可是有好多宝贝啊，想要的话就进去瞧瞧吧。 。。。

	OPTION 300 #参加欢乐嘉年华
	talk
endproc
proc 300 
;	GET_PLAYER_ID 1 50
;	ID_PLAYER_BASE_ATT @50 1 1 210 1 
;	if 210 >= 40
		GET_EVENT_STAT 401 8		
	
		IF 401 = 1
			enter_pworld 201 0 12
		ELSE
			add_sys_msg_start
			add_sys_msg 活动还没开始，无法进入！
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
;	else
;		add_sys_msg_start
;		add_sys_msg 你的等级小于40级，不能进入！！
;		set_sys_msg_mode 4 0 0
;		msg_info
;	
;	endif

endproc

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我的圣诞礼物！我的圣诞礼物！HOHOHO！老了我也要做圣诞老人！
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
			task_fini 3124 1
		endif
	endif
endproc 

	






