proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在我们幸福过活的同时，还有很多人缺衣少食，甚至连喝口干净的水都成了奢望，在此佳节之迹，时间城评价出的十佳劳模提供了众多商品在此义卖，希望能让更多的人逃离饥饿的阴影。
	add_talk #b#b#cff4e00除了义卖，我还在为灾区募捐，对于所有奉献爱心的人都会获得纪念品一份，也许不值几个钱，却是我的一点心意。
	add_talk #b#b#cFF00805月10日，艾陶就会离开时间城，将筹集的善款与物资送往灾区。

	option 150 #(买卖)劳模义卖商店
	
	GET_PLAYER_DAILY_FLAG 0 12 10
	if 10 = 0
	option 200 #我要捐赠10000绑银
	endif
	
	option 210 #我要捐赠（过期）时装一件
	talk

endproc 

proc 150
	OPEN_NPC_SHOP 
endproc 

proc 200
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 10000 and @ret >= 10000
		choose 1 201 202 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 10000
		choose 1 201 202 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 201
	endif
endproc

proc 201
	DEC_MONEY_TEST 0 10000 21 10
	if 10 = 0
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 12 1
			if Ret = 0
				DEC_MONEY 0 10000 21 12
				if 12 = 0
					ADD_BIND_ITEM_NUM_INPAK 5052306 1 1 13
					add_sys_msg_start
					add_sys_msg 多谢您的捐赠，送您一个劳动硕果以示感谢。
					set_sys_msg_mode 3 0 0
					msg_info
				endif
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

endproc

proc 210
	dialog 3
	add_talk $_npc$:
	add_talk #b#b您可以捐献除了阳光海滩比基尼以外的时装，过期的也可以。
	add_talk #b#b#c6d00d2背饰不可捐献
	add_talk #b#b
	add_talk #b#b将需要提交的物品拖至下方的框内
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret flag weizhi
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	ARM_IS_FASHION 10 @itemId
	if 10 = -1
		add_sys_msg_start
		add_sys_msg 请捐献除背饰、沙滩比基尼以外的时装，过期的也可以！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_WEAR_POS1 20 @itemID weizhi
	if @20 = 0 and weizhi = 24
		add_sys_msg_start
		add_sys_msg 请捐献除背饰、沙滩比基尼以外的时装，过期的也可以！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;排除泳装
	var flag 1
	if itemId = 3050010 or itemId = 3050011 or itemId = 3050012 or itemId = 3450010 or itemId = 3450011 or itemId = 3450012 or itemId = 3451010 or itemId = 3451011 or itemId = 3451012 or itemId = 3750010 or itemId = 3750011 or itemId = 3750012 or itemId = 3751010 or itemId = 3751011 or itemId = 3751012
		var flag 0
	endif
	
	; if itemId = 3653000 or itemId = 3653001 or itemId = 3653002 or itemId = 3653003 or itemId = 3653004 or itemId = 3653005 or itemId = 3653006 or itemId = 3653007 or itemId = 3650003
		; var flag 0
	; endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 这件时装太凉爽了，还请捐献一些比较实用、保暖的时装吧！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5052306 1 1 13
		
		add_sys_msg_start
		add_sys_msg 多谢您的捐赠，送您一个劳动硕果以示感谢。
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc