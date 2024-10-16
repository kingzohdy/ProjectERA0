;牧牛决赛,浩东
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在我很小的时候，爷爷带我去海滩上玩，他随手扔了一粒沙子在沙滩上，问我能否把他刚刚扔掉的那粒沙子找回来？
	add_talk #b#b我问他那是一粒什么样的沙子？
	add_talk #b#b他说那是一粒很完美很优秀的沙子。
	add_talk #b#b我用了很长时间，终是没能找到它。
	add_talk #b#b如今很多年过去了，我在不知不觉间也变成了沙滩中的一粒沙。
	add_talk #c186506牛仔很忙-决赛，活动时间安排如下：
	add_talk #b#b#c186506每周一，晚上20：30——21：00
	add_talk #b#b#c186506每周三，晚上20：30——21：00#n
	add_talk #b#b参加牛仔很忙决赛需要星光大道入场券，只有在初赛表现优异的人才能获得此券，获得进入星光大道的资格。
	
	option 100 #参加牛仔很忙决赛
	
	talk
endproc
proc 100
	cow_pworld_staus 1
	if 1 != 1
		add_sys_msg_start
		add_sys_msg 很抱歉，你现在还不能进去，因为现在不是活动时间！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar Ret
	GET_VAILD_ITEM_NUM_INPAK 5052207 1 Ret
	if Ret < 1
		add_sys_msg_start
		add_sys_msg 真是遗憾，只有拥有星光大道入场券的人才能进入其中参与决赛！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	COW_PWORLD_ENTER 1
	
	if 1 = 0
		DEC_VAILD_ITEM_NUM_INPAK 5052207 1 Ret
	else
		
	endif
endproc
