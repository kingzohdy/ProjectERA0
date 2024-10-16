;npc 安迪

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b金秋十月，丹桂飘香，在这美好的时刻，时间城里一片欢声笑语，此时此刻，我不禁心生感慨，感谢那些逝去的正在无私奉献的英雄，留给我们金碧辉煌的光明盛世。
	
		option 200 向所有为自由、未来而奋斗的人致敬
		option 300 【星辉宝钻】兑换【金卷】
		
		TASK_ADD_OPTION 4634 0 100
		TASK_ADD_OPTION 4636 0 101
	talk
endproc 

proc 100
	TASK_DO_OPTION 4634 0
endproc

proc 101
	TASK_DO_OPTION 4636 0
endproc


proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b时间和生命都在觉醒，魔法和命运正交织如错，迅速蜕变。
	add_talk #b#b欢迎前来祈福，为了逝去的英雄，为了正在为自由、未来而奋斗的勇士，为了我们所珍爱的人！
	add_talk #b#b
	add_talk #b#b#c186506祈福可获得 吉星高照（开启获得珍稀道具） #n
  
		GET_PLAYER_DAILY_FLAG 0 14 22
		if @22 = 0
			option 204 祈福（每天一次，需要99英雄纪念币）
		endif
		option 202 祈福（需要99金币）	
		option 203 祈福（需要999金币）
		option 1 返回
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 您的背包已满，请先清理至少预留一个格子
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 208 200 祈福需要#cb496ff消耗99金币#n，点击“确定”继续\n成功后可获得9个吉星普照！
endproc

proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 您的背包已满，请先清理至少预留一个格子
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 209 200 祈福需要#cb496ff消耗999金币#n，点击“确定”继续\n成功后可获得95个吉星普照！
endproc

proc 204
	GET_PLAYER_DAILY_FLAG 0 14 11
	;0表示未设置
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 您的背包已满，请先清理至少预留一个格子
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 210 200 祈福需要#cb496ff消耗99英雄纪念币#n，点击“确定”继续\n成功后可获得1个吉星高照！
endproc

proc 208
	DEC_MONEY_TEST 0 99 60 11
	if @11 = 0
		DEC_MONEY 0 99 60 10
		if @10 = 0
			add_sys_msg_start
			add_sys_msg 众神在聆听，愿你吉星普照！
			set_sys_msg_mode 3 0 0
			msg_info
			
			ADD_ITEM_NUM_INPAK 5060051 9 1 14 
			if @14 = 0
				add_sys_msg_start
				add_sys_msg 吉星普照已经放在您的背包中，祝您国庆快乐！
				set_sys_msg_mode 3 0 0
				msg_info
				
				add_sys_msg_start
				add_sys_msg $_player$ 在祈福中得到回应，获得了吉星普照
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 金币不足
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 209
	DEC_MONEY_TEST 0 999 60 11
	if @11 = 0
		DEC_MONEY 0 999 60 10
		if @10 = 0
			add_sys_msg_start
			add_sys_msg 众神在聆听，愿你吉星普照！
			set_sys_msg_mode 3 0 0
			msg_info
			
			ADD_ITEM_NUM_INPAK 5060051 95 1 14 
			if @14 = 0
				add_sys_msg_start
				add_sys_msg 吉星普照已经放在您的背包中，祝您国庆快乐！
				set_sys_msg_mode 3 0 0
				msg_info
				
				add_sys_msg_start
				add_sys_msg $_player$ 在祈福中得到回应，获得了吉星普照
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 金币不足
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 210
	GET_ITEM_NUM_INPAK 5060050 13 15
	if @13 = 0 and @15 >= 99
		DEC_ITEM_NUM_INPAK 5060050 99 16
		if @16 = 0
			add_sys_msg_start
			add_sys_msg 你的祷告直达云霄，愿你吉星高照
			set_sys_msg_mode 3 0 0
			msg_info
			
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 14 1
			if Ret = 0
				ADD_BIND_ITEM_NUM_INPAK 5060052 1 1 14 
				if @14 = 0
					add_sys_msg_start
					add_sys_msg 吉星高照已经放在您的背包中，祝您国庆快乐！
					set_sys_msg_mode 3 0 0
					msg_info
					
					add_sys_msg_start
					add_sys_msg $_player$ 在祈福中得到回应，获得了吉星高照
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 英雄纪念币数量不足
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只要你有足够的【#c005ebe星辉宝钻#n】，就能跟我兑换同等价值的【#cff4e00金卷#n】。
	add_talk #b#b
	add_talk #c186506兑换比率：
	add_talk #b#b1个【星辉宝钻】兑换10【金卷】#n
	
		option 301 1个【星辉宝钻】兑换10【金卷】
		option 302 10个【星辉宝钻】兑换100【金卷】
	talk
endproc

proc 301
	strvar ret0 num0 ret9 pmid
	GET_ITEM_NUM_INPAK 5200020 ret0 num0
	if ret0 = 0 and num0 > 0
		DEC_ITEM_NUM_INPAK 5200020 1 ret9
		if ret9 = 0
			GET_PLAYER_ID 1 pmid
			add_money @pmid 10 61 1
			
			add_sys_msg_start
			add_sys_msg 你已成功兑换了10金卷，[快捷键V]可打开商城查看
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上的星辉宝钻不足，暂时无法兑换
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 302
	strvar ret0 num0 ret9 pmid
	GET_ITEM_NUM_INPAK 5200020 ret0 num0
	if ret0 = 0 and num0 >= 10
		DEC_ITEM_NUM_INPAK 5200020 10 ret9
		if ret9 = 0
			GET_PLAYER_ID 1 pmid
			add_money @pmid 100 61 1
			
			add_sys_msg_start
			add_sys_msg 你已成功兑换了100金卷，[快捷键V]可打开商城查看
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上的星辉宝钻不足，暂时无法兑换
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
