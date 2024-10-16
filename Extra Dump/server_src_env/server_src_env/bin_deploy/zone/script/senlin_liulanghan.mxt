

proc 1
	dialog 1
	add_talk $_npc$:

		add_talk #b#b作为一名专业的流浪汉,我的装扮也必须专业的.超越传说中的犀利哥,是我最大的追求!我听说你们有一种能够变身的药水,变身效果非常犀利啊.
		add_talk #b#b如果你们能给我一瓶变身药水,作为交换,我可以帮你们引开驭兽者的小动物,让你们可以避免很多麻烦,直接挑战驭兽者.
		add_talk #b#b当然,直接给钱的话,我也不会反对的.
	
		option 150 #给流浪汉一瓶变身药水 
		option 160 #药水太麻烦,还是直接给你100银币吧
		option 170 #我自己能搞定,不劳您大驾!

	talk
	
endproc 
proc 150

	dialog 3
	add_talk $_npc$:
	add_talk #b#b请提交一瓶变身药水
	add_talk #b#b

		NPC_COMMIT_OPT 151
		
	talk
endproc 
proc 151
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

	if m_ID = 5006100 or m_ID = 5006101 or m_ID = 5006102 or m_ID = 5006103 or m_ID = 5006106
		switch m_ID
			case 5006100
				DEC_ITEM_NUM_BYIDX 5006100 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006101
				DEC_ITEM_NUM_BYIDX 5006101 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006102
				DEC_ITEM_NUM_BYIDX 5006102 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006103
				DEC_ITEM_NUM_BYIDX 5006103 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006106
				DEC_ITEM_NUM_BYIDX 5006106 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
		endswitch
	else
	
		add_sys_msg_start
		add_sys_msg 欺负我老人家眼神不好?你明明没有变身药水!
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 
proc 152


	;获得npc实例冒泡
	strvar mid roc
	GET_MAP_NPC_ID 304012 mid 1
	mon_speech @mid 0 2 1 我这就去把驭兽者的小动物拐走~
	
	del_npc 304012 1
	;删npc
	call 199
	;删怪物
	get_pworld_var 101 8 1 roc
	var roc 2
	set_pworld_var 101 8 @roc 1
	
	get_pworld_var 104 8 1 44
	var 44 0
	set_pworld_var 104 8 @44 1

	;标记，直接刷BOSS
	get_pworld_var 108 8 1 50 
	var 50 1
	set_pworld_var 108 8 @50 1
endproc
	
	
proc 160
	strvar moy
	DEC_MONEY_TEST 0 100 20 moy
	if moy = -1
		
		add_sys_msg_start
		add_sys_msg 兜里没钱,就别装大爷!
		set_sys_msg_mode 3 0 0
		msg_info
	else
		dec_money 0 100 20 50
		if 50 = 0 
			call 152
		endif
	endif
	
endproc 
proc 170
	return
endproc
proc 199
	;专家
	DELETE_MON_BY_DEFID 201276 10
	DELETE_MON_BY_DEFID 201281 1
	DELETE_MON_BY_DEFID 201280 1
	DELETE_MON_BY_DEFID 201279 1
	DELETE_MON_BY_DEFID 201278 1
	DELETE_MON_BY_DEFID 201277 1
	DELETE_MON_BY_DEFID 201282 1
	DELETE_MON_BY_DEFID 201283 1
	DELETE_MON_BY_DEFID 201284 1
	DELETE_MON_BY_DEFID 201285 1
	
	;困难
	DELETE_MON_BY_DEFID 201206 10
	DELETE_MON_BY_DEFID 201215 1
	DELETE_MON_BY_DEFID 201211 1
	DELETE_MON_BY_DEFID 201210 1
	DELETE_MON_BY_DEFID 201209 1
	DELETE_MON_BY_DEFID 201208 1
	DELETE_MON_BY_DEFID 201207 1
	DELETE_MON_BY_DEFID 201212 1
	DELETE_MON_BY_DEFID 201213 1
	DELETE_MON_BY_DEFID 201214 1
	
	
	; add_sys_msg_start
	; add_sys_msg $@10$ 
	; set_sys_msg_mode 1 0 0
	; msg_info
endproc 






