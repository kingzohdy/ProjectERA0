;
; 携带任务\副本503

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每个人都会经过这个阶段，见到一座山，就想知道山后面是什么，大部分人会因为山势险峻或害怕失望而绕行。我很想告诉他，如果想知道山那边有什么，就亲自去看看吧，也会有意外的收获。

	;五一活动
	task_stat 4433 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	task_stat 1351 1 52
	if 52 = 2
		task_fini 1351 1
	endif
	
;	task_add_option 1089 1 112
	task_add_option 1090 1 113
	task_add_option 1091 1 114
	task_add_option 1092 1 115
	task_add_option 1093 1 116
	
	task_add_option 1084 1 117
	task_add_option 1085 1 118
	task_add_option 1088 1 119
	task_add_option 1086 1 120
	task_add_option 1087 1 121
	task_add_option 1089 1 122
	 

	task_add_option 1094 1 123
	
	strvar Ret
	strvar rum
	GET_ITEM_NUM_INPAK 5300966 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #孩子们在等着你的红包呢

	endif
	
	
	strvar Ret513
	TASK_STAT 1092 1 Ret513
	
	if Ret513 = 2
		option 160 #逆往之间
	endif
	strvar ret
	task_stat 1088 1 ret
	if ret = 2
		OPTION 180 开始答题
	endif
	
	option 130 #打开商店
	option 170 修理强力机甲兽
	option 200 装备分解（晶石获得）示意图
	option 201 镶嵌晶石（增加属性）示意图
	option 202 晶石吸收（晶石升级）示意图

	talk

endproc

proc 112
    task_do_option 1089 1
endproc
proc 113
    task_do_option 1090 1
endproc
proc 114
    task_do_option 1091 1
endproc
proc 115
    task_do_option 1092 1
endproc
proc 116
    task_do_option 1093 1
endproc

proc 117
    task_do_option 1084 1
endproc
proc 118
    task_do_option 1085 1
endproc
proc 119
    task_do_option 1088 1
endproc

proc 120
    task_do_option 1086 1
endproc
proc 121
    task_do_option 1087 1
endproc
proc 122
    task_do_option 1089 1
endproc
proc 123

    task_do_option 1094 1
endproc

proc 130
	task_stat 1094 1 20
	if 20 = 2
		task_fini 1094 10
	endif
	
	OPEN_NPC_SHOP 
endproc 
proc 160
	enter_pworld 513 0 1
endproc 

proc 170
;	CHOOSE 1 171 172 修理强力机甲兽需要消耗50000银币，确定需要修理
	dialog 1
	add_talk $_npc$:
	add_talk #b#b哎，不得不承认，驱龙真是越来越脆弱了，如今居然损坏了……我并不是责怪你弄坏了驱龙，只是感叹……你懂的！！
	add_talk #b#b我当然可以修复驱龙，不过，你需要付出一点小小的代价。 
	add_talk #b#b#cF93500修理费用：5000绑银#n（先花费绑银，绑银不足则花费银币）	

		option 171 #修理
	talk
endproc 
proc 171
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 5000 and @ret >= 5000
		choose 1 173 172 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 5000
		choose 1 173 172 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 173
	endif
endproc 
proc 172
	return
endproc

proc 173	
	strvar Num Retm
	GET_ITEM_NUM_INPAK 5300883 1 Num
	if Num > 0 
		strvar retmid
		GET_ROLE_RIDE_ON_MACHINE 0 retmid
		if retmid = 0
			DEC_MONEY_TEST 0 5000 21 Retm 
				
			if Retm = -1
				ADD_SYS_MSG_START
				ADD_SYS_MSG 银币不足，修理失败!
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			else
				strvar Ret
				DEC_ITEM_NUM_INPAK 5300883 1 Ret
				if Ret = 0 
					strvar Retitem
					add_item_num_inpak 5300883 1 1 Retitem
					if Retitem = 0
						DEC_MONEY 0 5000 21 10	
					endif
				endif
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG 机械在骑乘状态，不可修理！
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 修理失败，您背包内没有需要修理的强力机甲兽。
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 


proc 2
	strvar Retask
	get_trig_task 1 Retask
	if @Retask = 1092
		dialog 1
		add_talk $_npc$:
		add_talk #b#b每个人都会经过这个阶段，见到一座山，就想知道山后面是什么，大部分人会因为山势险峻或害怕失望而绕行。我很想告诉他，如果想知道山那边有什么，就亲自去看看吧，也会有意外的收获。
		OPTION 160 #逆往之间
		talk
	endif
	if @Retask = 1088
		dialog 1
		add_talk $_npc$:
		add_talk #b#b每个人都会经过这个阶段，见到一座山，就想知道山后面是什么，大部分人会因为山势险峻或害怕失望而绕行。我很想告诉他，如果想知道山那边有什么，就亲自去看看吧，也会有意外的收获。
		OPTION 180 开始答题
		talk
	endif
endproc

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 1、关于分解装备，以下说法正确的是？
		option 181 A、10级以下的装备不可以分解
		option 181 B、白色装备不可以分解
		option 181 C、分解装备有几率获得晶石

		option 182 D、以上说法全对
		
	talk
endproc 

proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#b其他两条说法也对哟！
	add_talk #b#b
	add_talk #b#b #c0181f9 10级以上的绿色、蓝色、紫色、橙色装备可以被分解，分解时一定获得材料，有几率获得晶石，晶石可镶嵌到能源系统中，提升人物属性。#n
		option 180 重新回答

	talk
endproc 
proc 182
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 2、晶石的属性是随机的，并且数值也有波动，请问，晶石的属性来源？

		option 185 A、系统随机生成
		option 186 B、继承了被分解前装备的附加属性
		option 185 C、想什么来什么
		option 185 D、不知道
		
	talk
endproc 

proc 185
	dialog 1
	add_talk $_npc$:
	add_talk #b#b正确的答案是：
	add_talk #b#b继承了被分解前装备的附加属性

	add_talk #b#b#c0181f9通过分解获得的晶石，会继承装备的附加属性，变成晶石的属性。#n
	add_talk #b#b
	add_talk #b#b#c0181f9例如：你拥有一件装备有2条附加属性：生命+1000、防御+800，那么你分解到的晶石的附加属性要么是生命，要么是防御。#n

		option 182 重新回答

	talk
endproc 
proc 186
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 3、晶石1吸收了经验后，再被其他晶石吸收，晶石1所吸收的经验会被扣掉吗？

		option 190 A、会
		option 191 #B、不会
		option 190 C、不清楚
		
	talk
endproc 
proc 190
	dialog 1
	add_talk $_npc$:
	add_talk #b#b正确的答案是：
	add_talk #b#b晶石已吸收的经验不会被扣掉，会如数被下一颗晶石吸收
	add_talk #b#b
	add_talk #b#b#c0181f9举例：你拥有了一颗3级四阶绿晶石（假设已吸收了1千点经验值升为3级），又得到了一颗属性更好的1级四阶绿晶石，这颗1级晶石吸收了3级晶石后，会直接变成3级（会吸收之前的1000点经验值+四阶绿晶石的经验值）。#n

		option 186 重新回答

	talk
endproc 

proc 191
	task_fini 1088 1
endproc 

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300966 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300966 1 1 20
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
			task_fini 4433 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b平安为你开道，阳光把你普照，幸福与你相邀，健康把你拥抱，快乐与你嬉闹，好运投怀送抱，朋友对你关照，祝你五一称心如意，日子乐逍遥。
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













