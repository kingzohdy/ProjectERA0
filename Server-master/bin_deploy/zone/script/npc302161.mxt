;npc啊姆拉

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b各位大朋友，小朋友，男朋友，女朋友，帅朋友，酷朋友，快来看看我的神奇玩具屋吧，这里有世界各地的新奇玩具、轻松益智的小游戏，还有囊括众多稀奇道具的潘多拉宝盒！
	add_talk #b#b
	add_talk #b#b#cff00806月7日晚上24点整，阿姆拉就会离开时间城，继续寻找各式各样的玩具充实玩具屋。

	option 105 #(买卖)玩具商店
	
	GET_PLAYER_DAILY_FLAG 0 14 10
	if 10 = 0
	option 200 #1000绑银换一个潘多拉宝盒
	endif
	
	option 210 #一件（过期）时装换一个潘多拉宝盒

	option 214 神奇泡泡屋
	talk
endproc 

proc 105
	OPEN_NPC_SHOP 
endproc 

proc 200
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 201 202 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 201 202 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
	;绑银足了
		call 201
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 1000 21 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 14 1
			if Ret = 0
				DEC_MONEY 0 1000 21 12
				if 12 = 0
					ADD_ITEM_NUM_INPAK 5052116 1 1 13
					add_sys_msg_start
					add_sys_msg 多谢您的捐赠，送您一个潘多拉魔盒以示感谢。
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				PLAYER_CLOSE_DIALOG1 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足。
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 202
	call 1
endproc

proc 210
	dialog 3
	add_talk $_npc$:
	add_talk #b#b您可以捐献时装，过期的也可以。
	add_talk #b#b背饰也可以捐献
	add_talk #b#b
	add_talk #b#b将需要提交的物品拖至下方的框内
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;背饰
	var flag 0
	if itemId = 3653000 or itemId = 3653001 or itemId = 3653002 or itemId = 3653003 or itemId = 3653004 or itemId = 3653005 or itemId = 3653006 or itemId = 3653007 or itemId = 3650003
		var flag 1
	endif
	
	if itemId = 3650004 or itemId = 3650005 or itemId = 3650006 or itemId = 3650007 or itemId = 3650015 or itemId = 3650017 or itemId = 3651001 
		var flag 1
	endif
	
	if itemId = 3651002 or itemId = 3651003 or itemId = 3651004 or itemId = 3651005 or itemId = 3651006 or itemId = 3651007 or itemId = 3652000
		var flag 1
	endif
	
	if itemId = 3652001 or itemId = 3652003 or itemId = 3652005
		var flag 1
	endif
	
	ARM_IS_FASHION 10 @itemId
	if 10 = -1
		if flag = 0
			add_sys_msg_start
			add_sys_msg 请捐献时装，过期的也可以！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
	    ADD_ITEM_NUM_INPAK 5052116 1 1 13
		
		add_sys_msg_start
		add_sys_msg 多谢您的捐赠，送您一个潘多拉魔盒以示感谢。
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc

;进入神奇泡泡小屋（单人副本）
proc 211
	ENTER_PWORLD 209 0 1
endproc


;进入神奇泡泡小屋（队伍副本）
proc 212
	ENTER_PWORLD 210 0 1
endproc

proc 214
	DIALOG 1
	add_talk $_npc$：
	add_talk 泡泡屋诀窍：
	add_talk #b#b当泡泡被打破(杀死)之后，就会变成另外一个泡泡，也就是说，泡泡屋里的泡泡总数量是不会减的!
	add_talk #b#b
	add_talk 以下是泡泡衔变规则：
	add_talk #b#b#b#b#cff0080石头#n → #cff4e00剪刀 #n
	add_talk #b#b#b#b#cff4e00剪刀#n → #c0181f9包袱#n
	add_talk #b#b#b#b#c0181f9包袱#n → #cff0080石头#n
	add_talk #b#b
	add_talk 胜利条件：
	add_talk #b#b当计时结束时，泡泡屋里#c186506所有的泡泡归于同一类型(颜色)
	option 215 进入神奇泡泡小屋（单人副本）
	option 212 #进入神奇泡泡屋（队伍副本）
	
	talk
endproc

proc 215
	choose 1 211 216 神奇泡泡小屋（单人副本），每天只可进入1次，是否立刻进入？
endproc

proc 216
	call 214
endproc
