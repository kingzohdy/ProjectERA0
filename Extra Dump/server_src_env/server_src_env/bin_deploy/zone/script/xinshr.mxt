;商人 
; 携带任务\打开商店

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们的口号是：不求最贵，但求最好！

	
;	strvar Ret1053 
;	TASK_STAT 1053 1 Ret1053
;	if Ret1053 = 2
;		option 150 我来领取急救喷剂
;	endif
	
	OPTION 110 #(买卖)打开商店 
	
	
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300762 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	talk
endproc
proc 110
	OPEN_NPC_SHOP
endproc 

proc 150
	DIALOG 1
	add_talk $_npc$:
	strvar Gend
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	if Gend = 1
		add_talk #b#b呀，帅哥，以前没见过你啊，是今天才加入自由联盟的么？要不要买点食物啊？我给你打八折！什么，是罗纳度让你来领急救喷剂的，怎么不早说呢，看在你是个帅哥的面子上，就送你了，不要钱。不过下次如果你要买食物，一定记得来找我哟。
	else
		add_talk #b#b呀，美女，以前没见过你啊，是今天才加入自由联盟的么？要不要买点食物啊？我给你打八折！什么，是罗纳度让你来领急救喷剂的，怎么不早说呢，看在你是个美女的面子上，就送你了，不要钱。不过下次如果你要买食物，一定记得来找我哟。

	endif
		option 151 #多谢了，我先去送喷剂，回头聊

	talk

endproc 
proc 151
	
	ADD_ITEM_NUM_INPAK 5300004 1 1 0

endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货殖列传，嗯？克尔泽先生的批注本。
	add_talk #b#b和我交换吧，我这里的随便拿出来一本都值上万块。
	
		option 501 #交换

	talk
endproc

proc 501
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300762 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300762 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300763 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300764 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300765 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300766 1 0 0
		;	endif
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





