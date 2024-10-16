

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b我在这里,是因为我杀人的手段高明,你们进来这里,是因为你们还不够强!
	add_talk #b#b在我的地盘,就得听我的! 
	add_talk #b#b杀气值达到30点,低于200点,将会红名,这么一点杀气值是没资格进入这里的.当然,花点小钱,你也可以来这里开开眼界.红名状态,死亡是不会掉落经验的,但是看到守卫,你最好是像兔子一样的逃跑吧!
	add_talk #b#b杀气值达到200以上,恭喜你,你已经红的发黑了.你的复活点就是这强者和人渣的集中营,杀人恶魔和冲动者的聚集地.杀气值过高的时候,将会陷入混乱杀戮的状态,招式凌乱,空门大开,面对其他玩家,你就会变得软弱并且毫无威胁!

	add_talk #b#b杀气值达到200以上,监狱就是你的家,使用回城卷轴,公会城市传送卷神马的,统统都会传送到我这里!

	;五一活动
	task_stat 4435 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif

	option 100 #打开商店
		
	option 101 #回到时间城
		
	talk
endproc
proc 100
	OPEN_NPC_SHOP 
	
endproc
proc 101
	strvar Ret
	GET_PLAYER_PKVAL_TYPE 0 Ret
	if Ret != 3
		TRANSMIT 1000 26100 28800 0
	else
		add_sys_msg_start
		add_sys_msg 你的杀气太重了，暂时不能让你回到时间城。
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4435 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b如果我有摇控器，就在人犯罪之前把他的心情换到幸福频道；如果我有时光机，就让时光停在相亲相爱。亲爱的，五一快乐！
			option 511 #没问题
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“礼品盒”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
