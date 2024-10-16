proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b我们基尔特商会主要经营贸易运输业，拥有着丰富矿产资源的落日荒野一带，自然成为我们运输的重点路线。如果你也想赚取丰厚的报酬，或许可以来协助运输，正好现在急缺人手中。
	add_talk #b
	add_talk #b#b#c186506商运[55级]-人物达到55级后可前来接取商运任务，完成后将获得丰厚的经验奖励

	add_talk #b#b#c0181f9敲击键盘上的“F12”键可以隐藏周围的玩家，使你快速找到自己的货车#n
	add_talk #b#b#c186506<拥有银星徽章特权，货车增加20%移动速度>
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300782 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 600 曲别针换别墅
		endif
	endif
	
	
	task_stat 4497 1 20
	if 20 = 2
		option 200 #向基尔特商会亨利赠送一个粽子
	endif
	
	;OPTION 101 #(买卖)声望商店
	

	strvar car
	get_dart_car_type 1 car
	if car != 6
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
		
		if ret = 0 and num > 0 
			option 401 #更换货车
		endif	
	else
		
		if car = 6
			task_stat 1096 1 20
			if 20 = 2
				option 450 #租赁货车
			endif
		endif
	endif
	strvar ret
	GET_ITEM_NUM_INPAK 5300945 1 num
	task_stat 1096 1 ret
	if ret = 2 and num > 0
		option 300 #提交商运物资
	endif	
	
	;单人
	;(新加商运“商运知多少？”任务——hnz）
	task_stat 1097 1 10
	if 10 = 2
	option 701 #我已准备就绪，随时可以提问
	endif
	
	strvar LV ret2
	GET_PLAYER_BASE_ATT 1 1 LV 1
	CLAN_IS_OPEN_DART 1 ret2
	; get_now_time 0 0 0 0 time 0 0

	task_add_option 1096 1 120
	task_add_option 1095 1 121

	;(新加商运“商运知多少？”任务——hnz）
	task_add_option 1097 1 799
	
	;(新加公会商运选项——hnz）
	if @LV >= 55
		option 150 #【公会商运】活动
	endif
	talk
endproc


; PROC 101
	; OPEN_NPC_SHOP
; ENDPROC
;;;;;单人

;(新加商运“商运知多少？”任务——hnz）
proc 799
	task_do_option 1097 1
endproc

proc 120
	task_do_option 1096 1
endproc
proc 121
	task_do_option 1095 1
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
	task_stat 4497 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4497 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4497 1
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




proc 300

	strvar retin retcar
	in_dart_car retin retcar 0
	if retin = 0 and retcar = 1
	
		strvar num
		GET_ITEM_NUM_INPAK 5300945 1 num
		if num > 0
			strvar ret
			dec_item_num_inpak 5300945 1 ret
			if ret = 0
				TASK_FINI 1096 1
			endif
			
		else
			add_sys_msg_start
			add_sys_msg 你身上没有这件物品！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif

	else
		add_sys_msg_start
		add_sys_msg 提交商运物资时，人物需要搭载在货车上
		set_sys_msg_mode 4 0 0
		msg_info
		
	endif
endproc

proc 400
	new_dart_car 1 600
endproc

proc 401
		
	dialog 1
	;add_talk $_npc$:
	add_talk 换车说明
	add_talk #b#b● 随机更换货车档次
	add_talk #b#b● 货车档次越高，商运最终的奖励越高
	add_talk #b#b● #c005ebe贵重#n\#c6d00d2富商#n\#cff4e00黄金#n货车额外奖励“蒸汽核心”
	add_talk #b#b● “蒸汽核心”可用于参与“混乱元素”活动
	add_talk #b#b● 折返路程商会将停止换车服务
	add_talk #b#b● 换车需要支付1份“换车合同”
	add_talk #b
	;add_talk 货车档次
	add_talk 普通货车——最终经验奖励500%
    add_talk #c0a6400小资货车#n——最终经验奖励600%
    add_talk #c005ebe贵重货车#n——最终经验奖励750%、蒸汽核心*1
    add_talk #c6d00d2富商货车#n——最终经验奖励1500%、蒸汽核心*2
    add_talk #cff4e00黄金货车#n——最终经验奖励3000%、蒸汽核心*4
	add_talk #b
	
	strvar car
	get_dart_car_type 1 car
	if car = 0
	add_talk 当前货车：当前无任何货车
	endif
	if car = 1 	
	add_talk 当前货车：普通货车
	endif
	if car = 2
	add_talk 当前货车：#c0a6400小资货车#n
	endif
	if car = 3
	add_talk 当前货车：#c005ebe贵重货车#n
	endif
	if car = 4
	add_talk 当前货车：#c6d00d2富商货车#n
	endif
	if car = 5
	add_talk 当前货车：#cff4e00黄金货车#n
	endif
	if car = 6
	add_talk 当前货车：破旧货车
	endif	
	
	option 402 我想更换运送的货车
	
	talk

endproc
proc 402
	task_stat 1096 1 ret
	if ret = 0
		add_sys_msg_start
		add_sys_msg 未接取商运任务，无法更换货车
		set_sys_msg_mode 3 0 0
		msg_info
		return
	ENDIF
	
	GET_ITEM_NUM_INPAK 5060023 100 101
	if 101 < 1
		BUY_HETONG 5060023
		return
	ENDIF
	
	strvar car
	get_dart_car_type 1 car
	if car = 5
		choose 2 403 403 当前已是最高档次的“黄金货车”
	else
		call 404
	endif
	
endproc 
proc 403
	
endproc 
proc 404
	strvar mid
	reset_dart_car mid
	GET_STAR_INFO 4 20
	
	target_add_status 3 @mid 3203 1 3600 1
	if 20 = 1
		strvar car
		get_dart_car_type 1 car
		if car = 1 or car = 2 or car = 3
		
			target_add_status 3 @mid 3202 2 3600 1
		endif
		if car = 4 or car = 5
			target_add_status 3 @mid 3202 3 3600 1
		endif
	endif
	call 401
endproc

proc 450
	dialog 1
	;add_talk $_npc$:
	add_talk 租车说明
	;add_talk #b#b● 货车租赁时间结束后，可重新租赁货车。
	add_talk #b#b● 货车损坏成破旧货车后，可重新租赁货车。
	add_talk #b#b● 重新租赁的货车为原有档次。
	;add_talk #b#b● 租赁的货车不再奖励与掉落“蒸汽核心”。
	;add_talk #b#b● 重新租赁货车时，商会自动回收破旧货车。	
	add_talk #b#b● 租赁需要支付相应数量的“换车合同”。
	add_talk #b#b
	add_talk 普通货车——换车合同*1
	add_talk #c0a6400小资货车#n——换车合同*1
	add_talk #c005ebe贵重货车#n——换车合同*2
	add_talk #c6d00d2富商货车#n——换车合同*4
	add_talk #cff4e00黄金货车#n——换车合同*6
	add_talk #b#b
	
	GET_PRE_DART_CAR 10 20
	GET_ITEM_NUM_INPAK 5060023 1 40
	if 10 = 0 
		if 20 = 1
			add_talk 原有档次：普通货车
			if 40 = 0
				add_talk 当前携带：换车合同* #cF80B0B $@40$#n
			else
				add_talk 当前携带：换车合同* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 2
			add_talk 原有档次：#c0a6400小资货车#n
			if 40 = 0
				add_talk 当前携带：换车合同* #cF80B0B $@40$#n
			else
				add_talk 当前携带：换车合同* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 3
			add_talk 原有档次：#c005ebe贵重货车#n
			if 40 < 2
				add_talk 当前携带：换车合同* #cF80B0B $@40$#n
			else
				add_talk 当前携带：换车合同* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 4
			add_talk 原有档次：#c6d00d2富商货车#n
			if 40 < 4
				add_talk 当前携带：换车合同* #cF80B0B $@40$#n
			else
				add_talk 当前携带：换车合同* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 5
			add_talk 原有档次：#cff4e00黄金货车#n
			if 40 < 6
				add_talk 当前携带：换车合同* #cF80B0B $@40$#n
			else
				add_talk 当前携带：换车合同* #c0a6400 $@40$#n
			endif
		endif
	endif

	add_talk #b#b
	add_talk #c0a6400便捷服务
	add_talk 重新租赁货车时，破旧货车无需开到租凭处。
	
	option 451 #我想重新租赁货车
	
	talk
	
endproc
proc 451
	GET_PRE_DART_CAR 10 20
	if 10 = 0 
		if 20 = 1
			RESUME_DART_CAR 10 1
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 2
			RESUME_DART_CAR 10 1
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 3
			RESUME_DART_CAR 10 2
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 4
			RESUME_DART_CAR 10 4
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 5
			RESUME_DART_CAR 10 6
			target_add_status 3 @10 3203 1 3600 1
		endif
	endif

endproc




proc 2
	get_trig_task 1 2
	if 2 = 1096
		
		RIDE_OFF 1
		strvar mid
		new_dart_car mid 3600
		
		target_add_status 3 @mid 3203 1 3600 1
		
		GET_STAR_INFO 4 20
		if 20 = 1
			strvar car
			get_dart_car_type 1 car
			if car = 1 or car = 2 or car = 3
				target_add_status 3 @mid 3202 2 3600 1
			
			endif
			if car = 4 or car = 5
				target_add_status 3 @mid 3202 3 3600 1
			endif
		endif
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我们基尔特商会主要经营贸易运输业，成员遍布世界各角落。
		add_talk #b#b而拥有丰富矿产资源的落日荒野一带，则划为我们重点的运输路线，只要客户支付足够费用，我们就保证物资安全抵达！
		add_talk #b#b#如果你想赚取丰厚的报酬，或许可以来协助我们运输，正好我们现在急缺人手。
		add_talk #b
		add_talk #b#b#c186506商运[55级]-人物达到55级后可前来接取商运任务，完成后将获得丰厚的经验奖励
		
		; OPTION 101 #(买卖)声望商店
		
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
		if ret = 0 and num > 0 
			option 401 #更换货车
		endif
		
		strvar ret
		GET_ITEM_NUM_INPAK 5300945 1 num
		task_stat 1096 1 ret
		if ret = 2 and num > 0
			option 300 #提交商运物资
		endif	
	
		talk
	endif
	
	;(新加商运“商运知多少？”任务——hnz）
	if 2 = 1097
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我们基尔特商会主要经营贸易运输业，拥有着丰富矿产资源的落日荒野一带，自然成为我们运输的重点路线。如果你也想赚取丰厚的报酬，或许可以来协助运输，正好现在急缺人手中。
		add_talk #b
		add_talk #b#b#c186506商运[55级]-人物达到55级后可前来接取商运任务，完成后将获得丰厚的经验奖励

		add_talk #b#b#c0181f9敲击键盘上的“F12”键可以隐藏周围的玩家，使你快速找到自己的货车#n
		add_talk #b#b#c186506<拥有银星徽章特权，货车增加20%移动速度>
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300782 1 num1
		if num1 > 0
			TASK_STAT 3304 1 Ret
			if Ret = 2
				option 600 曲别针换别墅
			endif
		endif
	
	
		; OPTION 101 #(买卖)声望商店
	
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
	
		if ret = 0 and num > 0 
			option 401 #更换货车
		endif
		strvar ret
		GET_ITEM_NUM_INPAK 5300945 1 num
		task_stat 1096 1 ret
		if ret = 2 and num > 0
			option 300 #提交商运物资
		endif	
		;单人
		task_stat 1097 1 10
		if 10 = 2
			option 701 #我已准备就绪，随时可以提问
		endif
		task_add_option 1096 1 120
		task_add_option 1095 1 121
		task_add_option 1097 1 801
		talk
		;if 2 = 30002
		;	task_fini 30002 1
		;endif
	endif

endproc 




proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b真没想到这本我寻觅数十年的《玉石考》竟然在你的手中，跟我换吧，你想要什么？别墅？
	add_talk #b#b<亨利沉思了一下>
	add_talk #b#b好吧，成交！
	option 601 #交换
		
	talk
endproc

proc 601
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300782 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300782 1 1
			ADD_ITEM_NUM_INPAK 5300785 1 0 0
			
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

proc 701
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运的货车是否分“普通、小资、贵重、富商、黄金”5类档次？
	option 801 #是
	option 901 #否
	talk
endproc

proc 702
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车档次越高，商运的最终奖励是否越丰厚？
	option 802 #是
	option 902 #否
	talk
endproc

proc 703
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运中是否无法找【亨利】更换货车，改变货车的档次？
	option 903 #是
	option 803 #否
	talk
endproc

proc 704
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运返程途中，是否还能找【亨利】继续更换货车？
	option 904 #是
	option 804 #否
	talk
endproc

proc 705
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运是否规定了货车的租赁时间？
	option 805 #是
	option 905 #否
	talk
endproc

proc 706
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车的租赁时间内未完成商运，商运是否会失败？
	option 806 #是
	option 906 #否
	talk
endproc

proc 707
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车是否难以抵御来自机械或炮弹的攻击？
	option 907 #是
	option 807 #否
	talk
endproc

proc 708
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车被摧毁后，是否无法找【亨利】继续更换货车？
	option 808 #是
	option 908 #否
	talk
endproc

proc 709
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车被摧毁后，是否无法找【亨利】重新租赁货车？
	option 909 #是
	option 809 #否
	talk
endproc

proc 710
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b重新租赁的货车是否为原有货车的档次？
	option 810 #是
	option 910 #否
	talk
endproc

proc 801
	call 702
endproc

proc 802
	call 703
endproc

proc 803
	call 704
endproc

proc 804
	call 705
endproc

proc 805
	call 706
endproc

proc 806
	call 707
endproc

proc 807
	call 708
endproc

proc 808
	call 709
endproc

proc 809
	call 710
endproc

proc 810
	task_fini 1097 10
	call 1
endproc

proc 901
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运的货车是否分“普通、小资、贵重、富商、黄金”5类档次？

	add_talk #b#b#c186506正确解答：商运的货车从低到高分为“普通、小资、贵重、富商、黄金”5类档次。#n
	option 801 #是
	option 901 #否
	talk
endproc

proc 902
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车档次越高，商运的最终奖励是否越丰厚？

	add_talk #b#b#c186506正确解答：货车的档次越高，商运可获得越丰厚的最终奖励。#n
	option 802 #是
	option 902 #否
	talk
endproc

proc 903
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运中是否无法找【亨利】更换货车，改变货车的档次？

	add_talk #b#b#c186506正确解答：商运可找【亨利】更换货车，改变货车的档次。#n
	option 903 #是
	option 803 #否
	talk
endproc

proc 904
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运返程途中，是否还能找【亨利】继续更换货车？

	add_talk #b#b#c186506正确解答：商运返程途中，无法找【亨利】继续更换货车。#n
	option 904 #是
	option 804 #否
	talk
endproc

proc 905
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b商运是否规定了货车的租赁时间？

	add_talk #b#b#c186506正确解答：商会严格规定了商运货车的租赁时间。#n
	option 805 #是
	option 905 #否
	talk
endproc

proc 906
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车的租赁时间内未完成商运，商运是否会失败？

	add_talk #b#b#c186506正确解答：租赁时间内未完成商运，商会将视为商运失败，并回收货车。#n
	option 806 #是
	option 906 #否
	talk
endproc

proc 907
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车是否难以抵御来自机械或炮弹的攻击？

	add_talk #b#b#c186506正确解答：货车坚固的外壳将有效抵御来自机械或炮弹的攻击。#n
	option 907 #是
	option 807 #否
	talk
endproc

proc 908
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车被摧毁后，是否无法找【亨利】继续更换货车？

	add_talk #b#b#c186506正确解答：货车被摧毁后，【亨利】不提供更换货车的服务。#n
	option 808 #是
	option 908 #否
	talk
endproc

proc 909
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b货车被摧毁后，是否无法找【亨利】重新租赁货车？

	add_talk #b#b#c186506正确解答：货车被摧毁后，【亨利】可提供租赁货车的服务。#n
	option 909 #是
	option 809 #否
	talk
endproc

proc 910
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b重新租赁的货车是否为原有货车的档次？

	add_talk #b#b#c186506正确解答：重新租赁货车将直接租赁原有档次的货车。#n
	option 810 #是
	option 910 #否
	talk
endproc

;(新加公会商运对话面板——hnz）
proc 150
	strvar ret pos time
	get_now_time 1 1 1 1 time 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b为了提高运输贸易的效率，基尔特商会与时间城议会签订了《协运合约》，凡是由公会组织的商运都可获得额外报酬。
	add_talk #b#b
	add_talk #c005ebe【公会商运】活动期间接取的【运送货车】任务，完成后将额外奖励大量经验值#n
	add_talk #b#b
	add_talk #c186506活动时间：每天1次(06:00-24:00)
	add_talk 会长开启：06:00-22:00
	add_talk 会员开启：22:00-24:00（前提：会长当天未开启）
	add_talk 持续时间：活动持续20分钟#n
	
	if time >= 6 and time < 24
		option 151 #我已召集会员，开启【公会商运】活动
	endif
	
	talk
endproc

;(新加公会商运接口——hnz）
proc 151
	strvar time ret pos art tab
	get_now_time 1 1 1 1 time 1 1
	PLAYER_CLAN_POS_INFO ret 0 0 0 pos
	CLAN_IS_OPEN_DART tab art
	
	if tab = 0 and art = 1
		add_sys_msg_start
		add_sys_msg 【公会商运】已开启或已结束，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time >= 6 and time < 22
		if ret = 0 and pos > 0
			if ret = 0 and ( pos = 7 or pos = 8 )
				choose 0 152 153 是否确认开启【公会商运】活动？
			else
				add_sys_msg_start
				add_sys_msg 22:00后会长若未开启，会员可自行开启【公会商运】活动
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会商运】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	elseif time >= 22 and time < 24
		if ret = 0 and pos > 0
			choose 0 152 153 是否确认开启【公会商运】活动？
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会商运】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 152
	strvar time ret pos art tab
	get_now_time 1 1 1 1 time 1 1
	PLAYER_CLAN_POS_INFO ret 0 0 0 pos
	CLAN_IS_OPEN_DART tab art
	
	if tab = 0 and art = 1
		add_sys_msg_start
		add_sys_msg 【公会商运】已开启或已结束，今天无法再开启活动
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time >= 6 and time < 22
		if ret = 0 and pos > 0
			if ret = 0 and ( pos = 7 or pos = 8 )
				CLAN_OPEN_DART 0
			else
				add_sys_msg_start
				add_sys_msg 22:00后会长若未开启，会员可自行开启【公会商运】活动
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会商运】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	elseif time >= 22 and time < 24
		if ret = 0 and pos > 0
			CLAN_OPEN_DART 0
		else
			add_sys_msg_start
			add_sys_msg 非公会成员无法开启【公会商运】活动
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc












