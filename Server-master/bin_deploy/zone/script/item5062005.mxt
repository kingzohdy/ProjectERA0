;中级藏宝图1

proc 1
	strvar Odds1 Odds2 Item1 Item2 Item3 Rand1
	var Item1 0
	var Item2 0
	;USE_TREASURE_PIC  #物品ID #有效范围 @返回值 @角度 @地图ID @X坐标 @Y坐标
	USE_TREASURE_PIC 5062005 1000 10 4 5 6 7

	GET_ITEM_ONEDAY_INFO 5062000 1 800 801
		;获得使用次数
	if 800 >= 20
		;使用20次以内
		add_sys_msg_start
		add_sys_msg 该物品当天使用次数已达到上限！！
		set_sys_msg_mode 4 0 0
		msg_info
	else

if 10 = 0
			;ADD_ITEM_ON_MAP  #第一个参数为返回值，第二个参数表示是否要客户端主动打开拾取面板 后面的依次 itemid_1,itemnum_1,itemid_2,itemnum_2,itemid_2,itemnum_2
			rand Odds1 100
			rand Rand1 2
					if Odds1 < 5
						var Item1 5221221
					endif
					if Odds1 >= 5 and Odds1 < 10
						var Item1 5221321
					endif
					if Odds1 >= 10 and Odds1 < 20
						var Item1 5221021
					endif
					if Odds1 >= 20 and Odds1 < 30
						var Item1 5222021
					endif
					if Odds1 >= 30 and Odds1 < 40
						var Item1 5222121
					endif
					if Odds1 >= 40 and Odds1 < 50
						var Item1 5221121
					endif
					if Odds1 >= 50 and Odds1 < 55
						var Item1 5222420
					endif					
					if Odds1 >= 55 and Odds1 < 60
						var Item1 5223320
					endif			
					if Odds1 >= 60 and Odds1 < 65
						var Item1 5229301
					endif			
					if Odds1 >= 65 and Odds1 < 70
						var Item1 5229501
					endif
					if Odds1 >= 70 and Odds1 < 80
						var Item1 5200003
					endif
					if Odds1 >= 80 and Odds1 < 85
						var Item1 5209003
					endif
					if Odds1 >= 85 and Odds1 < 95
						var Item1 5050210
					endif
					if Odds1 >= 95 and Odds1 < 100
						var Item1 5050510
					endif
			if Rand1 = 1
			rand Odds2 100
					if Odds2 < 5
						var Item2 5221221
					endif
					if Odds2 >= 5 and Odds2 < 10
						var Item2 5221321
					endif
					if Odds2 >= 10 and Odds2 < 20
						var Item2 5221021
					endif
					if Odds2 >= 20 and Odds2 < 30
						var Item2 5222021
					endif
					if Odds2 >= 30 and Odds2 < 40
						var Item2 5222121
					endif
					if Odds2 >= 40 and Odds2 < 50
						var Item2 5221121
					endif
					if Odds2 >= 50 and Odds2 < 55
						var Item2 5222420
					endif					
					if Odds2 >= 55 and Odds2 < 60
						var Item2 5223320
					endif			
					if Odds2 >= 60 and Odds2 < 65
						var Item2 5229301
					endif			
					if Odds2 >= 65 and Odds2 < 70
						var Item2 5229501
					endif
					if Odds2 >= 70 and Odds2 < 80
						var Item2 5200003
					endif
					if Odds2 >= 80 and Odds2 < 85
						var Item2 5209003
					endif
					if Odds2 >= 85 and Odds2 < 95
						var Item2 5050210
					endif
					if Odds2 >= 95 and Odds2 < 100
						var Item2 5050510
					endif
			endif
			ADD_ITEM_ON_MAP 8 1 @Item1 1 @Item2 1 0 0
			
			If 8 = 0
				ITEM_ADD_ONEDAY 5062000 1
				;使用次数+1
				dec_item_num_inpak 5062005 1 0

;				rand 21 49
;				If 21 = 49
;GET_MAP_MON_NUM #地图ID #怪物定义ID #返回标志 #怪物数量 #找到第一个怪物的实例ＩＤ 
;				GET_MAP_MON_NUM 2100 204080 0 23 0
;				GET_MAP_MON_NUM 2200 204080 0 24 0
;				GET_MAP_MON_NUM 2300 204080 0 25 0
;				GET_MAP_MON_NUM 2400 204080 0 26 0
;				GET_MAP_MON_NUM 2500 204080 0 27 0
;				If 23 = 0 And 24 = 0 And 25 = 0 And 26 = 0 And 27 = 0 
;					rand 22 9
;;NEW_MAP_MON #怪物id #怪物数量 #地图ID  #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #朝向 #生存时间 #返回标志 #第一只怪物的实例ID
;					If 22 = 0
;					NEW_MAP_MON 204080 1 2100 32000	6100 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，铜鼓村出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 1
;					NEW_MAP_MON 204080 1 2100 8000	23600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，铜鼓村出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 2
;					NEW_MAP_MON 204080 1 2200 9800	22800 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，雪原出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 3
;					NEW_MAP_MON 204080 1 2200 13800	20600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，雪原出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 4
;					NEW_MAP_MON 204080 1 2300 20700	2500 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，非洲沙漠出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 5
;					NEW_MAP_MON 204080 1 2300 19400	21700 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，非洲沙漠出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 6
;					NEW_MAP_MON 204080 1 2400 4600	21600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，北美玉米地出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;;					If 22 = 7
;					NEW_MAP_MON 204080 1 2400 20600	4000 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，北美玉米地出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 8
;					NEW_MAP_MON 204080 1 2500 12700	32100 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，南美玛雅出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 9
;					NEW_MAP_MON 204080 1 2500 12100	21500 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg 深藏的不稳定能量被触动，时空发生扭曲，南美玛雅出现了时空偷窃者！
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;				endif
;				endif

				add_sys_msg_start
				add_sys_msg 经过不懈努力，你终于获得了宝藏中的物品！
				set_sys_msg_mode 3 0 0
				msg_info

			Else
				add_sys_msg_start
				add_sys_msg 您的物品栏位置不足！
				set_sys_msg_mode 4 0 0
				msg_info
			endif
	endif

	If 10 = 1
	;GET_ITEM_USE_DUR #返回标志 #物品剩余使用次数 #物品最大使用次数
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 1
			add_sys_msg_start
			add_sys_msg 藏宝图已褶皱不堪，再次使用将被摧毁！
			set_sys_msg_mode 4 0 0
			msg_info
			endif
			If 700 = 0
			add_sys_msg_start
			add_sys_msg 藏宝图已损毁！
			set_sys_msg_mode 4 0 0
			msg_info
			endif
			If 700 != 0 Or 700 != 1
			add_sys_msg_start
			add_sys_msg 宝藏不在该地图，请到目标地图使用！
			set_sys_msg_mode 4 0 0
			msg_info
			endif
	endif

	If 10 = 2
	;GET_ITEM_USE_DUR #返回标志 #物品剩余使用次数 #物品最大使用次数
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 0
			add_sys_msg_start
			add_sys_msg 藏宝图已损毁！
			set_sys_msg_mode 4 0 0
			msg_info
			else

		If 4 = 1
			add_sys_msg_start
			add_sys_msg 宝藏在你的北方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 2
			add_sys_msg_start
			add_sys_msg 宝藏在你的东北方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 3
			add_sys_msg_start
			add_sys_msg 宝藏在你的东方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 4
			add_sys_msg_start
			add_sys_msg 宝藏在你的东南方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 5
			add_sys_msg_start
			add_sys_msg 宝藏在你的南方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 6
			add_sys_msg_start
			add_sys_msg 宝藏在你的西南方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 7
			add_sys_msg_start
			add_sys_msg 宝藏在你的西方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 8
			add_sys_msg_start
			add_sys_msg 宝藏在你的西北方，很近的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
			endif
			If 700 = 1
			add_sys_msg_start
			add_sys_msg 藏宝图已褶皱不堪，再次使用将被摧毁！
			set_sys_msg_mode 4 0 0
			msg_info
			endif
	endif

	If 10 = 3
	;GET_ITEM_USE_DUR #返回标志 #物品剩余使用次数 #物品最大使用次数
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 0
			add_sys_msg_start
			add_sys_msg 藏宝图已损毁！
			set_sys_msg_mode 4 0 0
			msg_info
			else

		If 4 = 1
			add_sys_msg_start
			add_sys_msg 宝藏在你的北方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 2
			add_sys_msg_start
			add_sys_msg 宝藏在你的东北方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 3
			add_sys_msg_start
			add_sys_msg 宝藏在你的东方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 4
			add_sys_msg_start
			add_sys_msg 宝藏在你的东南方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 5
			add_sys_msg_start
			add_sys_msg 宝藏在你的南方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 6
			add_sys_msg_start
			add_sys_msg 宝藏在你的西南方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 7
			add_sys_msg_start
			add_sys_msg 宝藏在你的西方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 8
			add_sys_msg_start
			add_sys_msg 宝藏在你的西北方，还有一段距离！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
			endif
			If 700 = 1
			add_sys_msg_start
			add_sys_msg 藏宝图已褶皱不堪，再次使用将被摧毁！
			set_sys_msg_mode 4 0 0
			msg_info
			endif

	endif

	If 10 = 4
	;GET_ITEM_USE_DUR #返回标志 #物品剩余使用次数 #物品最大使用次数
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 0
			add_sys_msg_start
			add_sys_msg 藏宝图已损毁！
			set_sys_msg_mode 4 0 0
			msg_info
			else

		If 4 = 1
			add_sys_msg_start
			add_sys_msg 宝藏在你的北方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 2
			add_sys_msg_start
			add_sys_msg 宝藏在你的东北方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
		If 4 = 3
			add_sys_msg_start
			add_sys_msg 宝藏在你的东方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 4
			add_sys_msg_start
			add_sys_msg 宝藏在你的东南方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 5
			add_sys_msg_start
			add_sys_msg 宝藏在你的南方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 6
			add_sys_msg_start
			add_sys_msg 宝藏在你的西南方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 7
			add_sys_msg_start
			add_sys_msg 宝藏在你的西方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		If 4 = 8
			add_sys_msg_start
			add_sys_msg 宝藏在你的西北方，遥远的地方！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
			endif
			If 700 = 1
			add_sys_msg_start
			add_sys_msg 藏宝图已褶皱不堪，再次使用将被摧毁！
			set_sys_msg_mode 4 0 0
			msg_info
			endif

	endif
	endif

endproc			