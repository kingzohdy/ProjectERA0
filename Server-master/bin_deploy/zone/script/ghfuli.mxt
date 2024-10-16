

proc 1
	DIALOG 1
	add_talk $_npc$:
	
	strvar Ret Pos Gend
	PLAYER_CLAN_POS_INFO Ret 0 0 0 Pos	
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	
	
	if Ret = 0 and Pos = 0
		add_talk #b#b这位小同志，你看起来好面生啊，第一次来领公会福利？别这么害羞嘛，走近点来，让小老头儿瞧瞧！
		add_talk #b#b咦，你的公会好个性啊，居然没有名字！啥，你还没有加入公会？那你赶紧找个公会申请加入吧，晚一天加公会就晚一天领福利哟，晚一天领公会福利，你就损失大大的哟。
	
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300934 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 曲别针换别墅
			endif	
		endif
	else
		if Gend = 1
			add_talk #b#b嗨，小伙子，你今天的公会福利好像还没领哟，虽然这福利不能让你一夜暴富、一夕成名，但起码也是小老头儿精心为你准备的，日积月累也不是一笔小数目啊！
		else	
			add_talk #b#b嗨，小姑娘，你今天的公会福利好像还没领哟，虽然这福利不能让你一夜暴富、一夕成名，但起码也是小老头儿精心为你准备的，日积月累也不是一笔小数目啊！
		endif
		
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300934 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 曲别针换别墅
			endif	
		endif
		option 150 #(福利)领取公会福利
		option 200 #(福利)领取官员福利
		
	endif
	
	
	
	;	option 160 #宝箱
	
	talk

endproc 
proc 160
	GET_ID_PLAYER_INFO 0 3 10
	GET_ID_PLAYER_INFO 0 4 20
	effect 0 0 playEffct($@10$,$@20$,"effect\\entity\\5300860.ent")
	
;	NEW_LIFE_NPC 1 307015 @10 @20 0 120 xbxiang_1.mac
	
endproc 
proc 200
	open_ui 84
endproc

proc 150
	strvar Ret Pos
	PLAYER_CLAN_POS_INFO Ret 0 0 0 Pos	
	if Ret = 0 and Pos != 0
		open_ui 62
	else
		add_sys_msg_start
		add_sys_msg 你已经不是公会成员了，不可打开公会福利面板。
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b别墅？我的确有，不过，不能给你，不然我住哪啊！
	add_talk #b#b小同志，要有耐心啊，别这么容易就气馁，我虽然不能拿别墅跟换，但我有插图版的《零纪元史》啊！世界上只有这才有哦！
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300934 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300934 1 1
			
			ADD_ITEM_NUM_INPAK 5300935 1 0 0

		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 




