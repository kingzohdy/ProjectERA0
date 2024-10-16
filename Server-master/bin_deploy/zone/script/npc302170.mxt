;npc 月饼

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我有一盒月饼，不知与谁能共？
	add_talk #b#b多少秘密在其中，路过之人岂懂？
	add_talk #b#b饼外情深意重，饼内美味重重。
	add_talk #b#b快来领月饼礼盒，共渡中秋佳节！
	add_talk #b#b
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		add_talk #b#b#c186506中秋活动期间，每天都可以来领取月饼礼盒，礼盒里的月饼那真叫一个丰富多彩，除了以下这些月饼以外还有一些出乎你意料之外的月饼哟！#n
		option 200 领取月饼礼盒
	elseif @13 = 0 and @12 < 40 
		add_talk #b#b#c186506你现在还小，等你40级了再来找我领取月饼礼盒吧！你可以通过做任务快速升级！既然来了，怎么能让你空手而回呢！送你5个欢喜月饼尝尝吧！#n
		GET_PLAYER_DAILY_FLAG 0 15 22
		if @22 = 0
			option 101 领取欢喜月饼
		endif
	else
	endif
		
		TASK_ADD_OPTION 4633 0 100
	talk
endproc 

proc 100
	TASK_DO_OPTION 4633 0
endproc

proc 101
	GET_PLAYER_DAILY_FLAG 0 15 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法领取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 15 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5012135 5 1 14 
		add_sys_msg_start
		add_sys_msg 中秋节快乐！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 200
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b每天早上6点重置在线时间，当天在线时间可累计。
	add_talk #b#b
	add_talk #b#b您今天的在线时间为：$@10$时$@11$分
	add_talk #b#b
	add_talk #b#b#c186506富贵月饼：完成任务后可获得银币
	add_talk #b#b冰皮月饼：用于中秋灯谜，完成后获得经验
	add_talk #b#b团圆月饼：可随时打开急你所急航空传送界面
	add_talk #b#b甜肉月饼：吃多了会变胖#n
	
	GET_PLAYER_DAILY_FLAG 0 9 20
	GET_PLAYER_DAILY_FLAG 0 12 21
	GET_PLAYER_DAILY_FLAG 0 14 22
	
	if @20 = 0
		option 201 领取在线月饼礼盒
	endif
	
	if @21 = 0
		option 202 领取在线满2小时月饼礼盒
	endif
	
	if @22 = 0
		option 203 领取在线满4小时月饼礼盒
	endif
		
	talk
endproc

;月饼礼盒x1
proc 201
	GET_PLAYER_DAILY_FLAG 0 9 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法领取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 9 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5052118 1 1 14 
		add_sys_msg_start
		add_sys_msg 月饼礼盒已经放在了你的背包里，快打开看看是什么口味的吧！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

;月饼礼盒x4
proc 202
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 12 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		add_sys_msg_start
		add_sys_msg 抱歉，您今天在线时间不足2小时，请稍后再来领取。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 背包空间不足，无法领取
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 12 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5052118 4 1 14 
			add_sys_msg_start
			add_sys_msg 月饼礼盒已经放在了你的背包里，快打开看看是什么口味的吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

;月饼礼盒x8+2
proc 203
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 14 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 14400
		add_sys_msg_start
		add_sys_msg 抱歉，您今天在线时间不足4小时，请稍后再来领取。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 背包空间不足，无法领取
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 14 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5052118 10 1 15
			add_sys_msg_start
			add_sys_msg 月饼礼盒已经放在了你的背包里，快打开看看是什么口味的吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
