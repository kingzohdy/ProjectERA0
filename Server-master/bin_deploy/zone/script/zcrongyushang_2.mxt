
;小奶哥
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b流年不利啊，这战场荣誉商店没法开了，希望改变能给我带来人气和财富。
	add_talk #b#b你只要参加并完成 #c186506指定的日常任务、赏善罚恶、别针换别墅、魔法熔炉、环组和主线副本#n 等任务或活动，就可以获得议会颁发的生存奖章。
	add_talk #b#b生存奖章会作为一种货币储存在人物界面奖章一页中，可以用于购买我这里出售的各种货物。
	
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300769 1 num1
		if num1 > 0
			TASK_STAT 3304 1 Ret
			if Ret = 2
				option 500 曲别针换别墅
			endif
		endif
		; option 150 #打开商店
		
		task_stat 4493 1 20
		if 20 = 2
			option 200 #向中级荣誉商人诺茨赠送一个粽子
		endif
		
		task_add_option 1600 1 100
		
		task_add_option 2266 1 101
		task_add_option 2267 1 102
		task_add_option 2268 1 103
		task_add_option 2269 1 104
		
		strvar LV
		GET_PLAYER_BASE_ATT 1 1 LV 1
		
		if @LV < 50
			option 301 #(买卖)【生存奖章】商店
		endif
		if @LV >= 50 and @LV < 60
			option 302 #(买卖)【生存奖章】商店
		endif
		if @LV >= 60 and @LV < 70
			option 303 #(买卖)【生存奖章】商店
		endif
		if @LV >= 70 and @LV < 80
			option 304 #(买卖)【生存奖章】商店
		endif
		if @LV >= 80
			option 305 #(买卖)【生存奖章】商店
		endif
		
		
	talk

endproc
proc 150
	OPEN_NPC_SHOP 

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
	task_stat 4493 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4493 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4493 1
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

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你现在不和我换，以后不要再来找我…

	option 501 #交换

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300769 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300769 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300770 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300771 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300772 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300773 1 0 0
			endif
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
	
 


PROC 100
	task_do_option 1600 1
ENDPROC

PROC 101
	task_do_option 2266 1
ENDPROC
PROC 102
	task_do_option 2267 1
ENDPROC
PROC 103
	task_do_option 2268 1
ENDPROC
PROC 104
	task_do_option 2269 1
ENDPROC

proc 301
	OPEN_SHOP_BY_ID 10010
endproc
proc 302
	OPEN_SHOP_BY_ID 10011
endproc
proc 303
	OPEN_SHOP_BY_ID 10012
endproc
proc 304
	OPEN_SHOP_BY_ID 10013
endproc
proc 305
	OPEN_SHOP_BY_ID 10014
endproc


