;70副本,隐藏关npc
proc 1
	;隐藏关npc对话标记,0未触发过,1触发过可以通过
	get_pworld_var 44 32 1 21
	if 21 > 0
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b你可以跳了。
		talk
		return
	endif	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我似乎需要点什么才能让你通过。
	option 100 给点什么
        talk
endproc
proc 100
	DIALOG 3
	add_talk $_npc$:
	add_talk #b#b你想给我什么？
	NPC_COMMIT_OPT 101
        talk
	
endproc
proc 101
	
	strvar m_ID m_Pos1 m_Pos2 m_Col 
	GET_COMMIT_ITEM_INFO 1 m_ID
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	if m_ID = 5015005 
		DEC_ITEM_NUM_BYIDX @m_ID 1 @m_Pos1 @m_Pos2 1
		PLAYER_CLOSE_DIALOG 0 0
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b你给对了东西，我让你过去。
		
		talk
		call 102
	else
		;测试tick用的,赋值为0关闭此功能
		get_pworld_var 20 32 1 15
		if 15 > 0
			DEC_ITEM_NUM_BYIDX @m_ID 1 @m_Pos1 @m_Pos2 1
			PLAYER_CLOSE_DIALOG 0 0
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b虽然你胡乱塞了点东西，但是为了测试，我还是让你过吧。
			call 102
			talk
		else
			PLAYER_CLOSE_DIALOG 0 0
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b你给错了东西，我不会让你过去的。
			
			talk
		endif
	endif
endproc
proc 102
	;隐藏关npc对话标记,0未触发过,1触发过可以通过
	get_pworld_var 44 32 1 21
	if 21 = 0
		var 21 1
		set_pworld_var 44 32 @21 1
	endif	
endproc
