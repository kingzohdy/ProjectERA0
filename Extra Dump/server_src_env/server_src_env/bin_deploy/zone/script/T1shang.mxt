;303036	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗯，看起来有些寒酸，你还需要努力啊！
	add_talk #b#b战魂徽章商店新品上架，包括 #c186506各种宝石、重置齿轮、进阶果实凝炼器#n 等上档次的东东哦~~
	add_talk #b#b隔壁的诺茨已经被我逼的改行了呢，（娇羞状），哎哟我好有做生意的头脑~~
	
	;五一活动
	task_stat 4432 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	strvar Ret
	
	strvar rum
	GET_ITEM_NUM_INPAK 5300967 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #我是替孩子们来拿红包的


	endif
	; OPTION 110 #(买卖)打开商店 
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 50
		option 201 #(买卖)【战魂徽章】商店
	endif
	if @LV >= 50 and @LV < 60
		option 202 #(买卖)【战魂徽章】商店
	endif
	if @LV >= 60 and @LV < 70
		option 203 #(买卖)【战魂徽章】商店
	endif
	if @LV >= 70 and @LV < 80
		option 205 #(买卖)【战魂徽章】商店
	endif	
	if @LV >= 80
		option 204 #(买卖)【战魂徽章】商店
	endif
	
	talk


endproc

proc 110
	 OPEN_NPC_SHOP
endproc

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300967 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300967 1 1 20
		endif
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
			task_fini 4432 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b愿劳动节给你带来运气更好一点，心情更乐一点，工作更顺一点，爱情更甜一点。
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

proc 201
	OPEN_SHOP_BY_ID 10030
	
	task_fini 2268 1
endproc
proc 202
	OPEN_SHOP_BY_ID 10031
	
	task_fini 2268 1
endproc
proc 203
	OPEN_SHOP_BY_ID 10032
	
	task_fini 2268 1
endproc
proc 204
	OPEN_SHOP_BY_ID 10033
	
	task_fini 2268 1
endproc
proc 205
	OPEN_SHOP_BY_ID 10034
	
	task_fini 2268 1
endproc
