
;测试

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b正宗好同志！正宗好背包！
	add_talk #b#b欢迎前来兑换“背包扣带”和“仓库钥匙”！
	add_talk #b#b本次兑换将由零纪元中最得意的同志为你们服务！
	add_talk #b#b只要你拥有足够的“扣带部件”即可获得各种扣带和钥匙！
	add_talk #b#b感谢时间城议会的大力支持！感谢各位的持续关注！
	add_talk #b#b
	add_talk #b#b#c186506使用背包扣带获得的扣带部件，可在此兑换背包扣带或仓库钥匙#n
	strvar RetTsk
	task_stat 3130 1 RetTsk
	
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	option 120 #背包商店
	
	task_stat 4498 1 20
	if 20 = 2
		option 250 #向栏位管理员布嘉兰赠送一个粽子
	endif
	
	option 300 #小背包换大背包

	
	talk
	
endproc 
proc 120
	OPEN_NPC_SHOP 
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每年圣诞老公公都会送礼物给我，$_player$，请你帮我谢谢他，我一定会努力，成为一个有作为的人！
		option 201 #交出圣诞礼物
	talk
endproc 
proc 201
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
			task_fini 3130 1
		endif
	endif
endproc 

proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
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
	task_stat 4498 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4498 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4498 1
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

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你给我5个小背包，我给你1个大背包，没有比这更划算的事了！对不对不对不？
	add_talk #b#b
	; add_talk #b#b#c1865065#n个【#c005ebe4格背包#n】可兑换#c1865061#n个【#c005ebe6格背包#n】
	add_talk #b#b#c1865065#n个【#c005ebe6格背包#n】可兑换#c1865061#n个【#c005ebe8格背包#n】
	add_talk #b#b#c1865065#n个【#c005ebe8格背包#n】可兑换#c1865061#n个【#c005ebe10格背包#n】
	add_talk #b#b#c1865065#n个【#c005ebe10格背包#n】可兑换#c1865061#n个【#c005ebe12格背包#n】
	add_talk #b#b#c1865065#n个【#c005ebe12格背包#n】可兑换#c1865061#n个【#c005ebe14格背包#n】
	
	; option 301 #我要换【6格背包】
	option 302 我要换【8格背包】
	option 303 我要换【10格背包】
	option 304 我要换【12格背包】
	option 305 我要换【14格背包】
	
	talk
endproc

proc 301
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251103 0 Num1
	GET_ITEM_NUM_INPAK 5251003 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251103 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251016 1 1 0
					
				add_sys_msg_start
				add_sys_msg 恭喜你成功兑换了1个【6格背包】！
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251103 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251003 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251016 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜你成功兑换了1个【6格背包】！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包不够哦，要5个【4格背包】才能换【6格背包】呢
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 302
	strvar Num ret
	GET_ITEM_NUM_INPAK 5251016 0 Num
	
	if Num >= 5
		DEC_ITEM_NUM_INPAK 5251016 5 ret
		if ret = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251104 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜你成功兑换了1个【8格背包】！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包不够哦，要5个【6格背包】才能换【8格背包】呢
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 303
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251104 0 Num1
	GET_ITEM_NUM_INPAK 5251004 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251104 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251105 1 1 0
					
				add_sys_msg_start
				add_sys_msg 恭喜你成功兑换了1个【10格背包】！
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251104 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251004 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251105 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜你成功兑换了1个【10格背包】！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包不够哦，要5个【8格背包】才能换【10格背包】呢
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 304
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251105 0 Num1
	GET_ITEM_NUM_INPAK 5251005 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251105 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251106 1 1 0
					
				add_sys_msg_start
				add_sys_msg 恭喜你成功兑换了1个【12格背包】！
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251105 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251005 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251106 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜你成功兑换了1个【12格背包】！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包不够哦，要5个【10格背包】才能换【12格背包】呢
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 305
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251106 0 Num1
	GET_ITEM_NUM_INPAK 5251006 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251106 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251107 1 1 0
					
				add_sys_msg_start
				add_sys_msg 恭喜你成功兑换了1个【14格背包】！
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251106 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251006 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251107 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜你成功兑换了1个【14格背包】！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的背包不够哦，要5个【12格背包】才能换【14格背包】呢
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
