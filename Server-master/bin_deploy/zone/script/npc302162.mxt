;npc布蕾丽

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b啊，这么容易就被找到了啊，下次我一定要找个谁也发现不了我的地方！究竟是哪儿呢？让我想想，嘘…我得好好想想…
	
	GET_PLAYER_DAILY_FLAG 0 12 10
	if 10 = 0
		option 200 听说你有好东西要给我？
	endif
	
	task_stat 4481 1 20
		if 20 = 2
			option 202 #哈哈，我抓到你了！
		endif
	talk
endproc 

proc 202
	task_fini 4481 1
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不算不算，这次不算啦，我要再藏一次，如果你还能找到我就算你厉害！如果你肯再陪我玩躲猫猫的话，我就送你5个粽叶，马上就要过端午节了，这可是包粽子必不可少的原料之一哟。
	
	option 201 #好吧，不过你得先把粽叶给我。
	talk
endproc

proc 201
	GET_PLAYER_DAILY_FLAG 0 12 10
	if @10 = 1
	;已领取
		add_sys_msg_start
		add_sys_msg 已领取
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		;strvar npcmid
		;GET_MAP_NPC_ID  302162 npcmid 1
		;MON_SPEECH  @npcmid 0 0 1 我今天已经给过你粽叶了，你都不跟我玩，哼，我也不跟你玩了！
	else
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else	
			SET_PLAYER_DAILY_FLAG 0 12 1
			
			ADD_BIND_ITEM_NUM_INPAK 5060041 5 1 13
			add_sys_msg_start
			add_sys_msg 获得五个粽叶
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
