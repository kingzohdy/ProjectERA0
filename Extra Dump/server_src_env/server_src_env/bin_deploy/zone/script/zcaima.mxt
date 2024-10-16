


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b送给我，那疲乏的和贫困的，挤在一起渴望自由呼吸的大众，那熙熙攘攘的岸上被遗弃的可怜的人群，那无家可归饱经风波的人们，一齐送给我，我站在时间城的繁华里，高举自由之魂的旗帜。
	add_talk #b#b
	add_talk #b#b自由之魂声望获得方式：通过击杀龙蛋谷、盘山道的怪物获得
	
;	strvar Ret 
;	task_stat 3676 1 Ret
;	if Ret = 2 
;		option 300 这里有张凭证需要你签名
;	endif
	
	option 150 #(买卖)声望商店
	task_stat 4496 1 20
	if 20 = 2
		option 200 #向自由之魂声望商人埃玛赠送一个粽子
	endif
	
	talk
	
endproc 	
proc 150
	OPEN_NPC_SHOP 
endproc

;proc 300
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b我就猜到是公会的领取凭证，搬物资时记得小心点，里面有不少易碎品。
;		option 301 #拿出领取凭证让埃玛签名
;
;	talk
;endproc 

proc 301
	GET_ITEM_NUM_INPAK 5301003 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5301003 1 30
		if 30 != -1
			ADD_ITEM_NUM_INPAK 5300879 1 1 0
			
		endif
	endif
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
	task_stat 4496 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4496 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4496 1
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

