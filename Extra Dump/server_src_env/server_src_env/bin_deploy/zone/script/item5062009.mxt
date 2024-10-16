;高级藏宝图

proc 1

	strvar d v 
	use_cangbaotu_pic 5062009 1000  d v
	switch d
	case 0
		ITEM_ADD_ONEDAY 5062000 1
		;使用次数+1
		dec_item_num_inpak 5062009 1 0
		
		GET_PLAYER_BASE_ATT 0 40 41 42
		GET_PLAYER_MAPINFO 1 1 1 43 44
		rand 50 100
		; if @50 < 70
		
		; add_sys_msg_start
		; add_sys_msg 运气不好，这个藏宝点被一伙职业挖宝的强盗捷足先登了
		; set_sys_msg_mode 4 0 0
		; msg_info

		; new_life_dyn_mon 2 200024 3 @43 @44 300 0 0 0 0 @41 0 60
		; new_life_dyn_mon 2 200025 3 @43 @44 300 0 0 0 0 @41 0 60
		; endif

		if @50 <= 65

		rand 10 7
		switch 10
			case 0
				NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao2.mac
			endcase
			case 1
				NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao3.mac
			endcase
			case 2
				NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao4.mac
			endcase
			case 3
				NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao1.mac
			endcase
			case 4
				NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao5.mac
			endcase
			case 5
				if 41 < 50
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao1_boss.mac
				endif
				if 41 >= 50 and 41 < 60
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao2_boss.mac
				endif
				if 41 >= 60 and 41 < 70
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao3_boss.mac
				endif
				if 41 >= 70 and 41 < 80
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao4_boss.mac
				endif
				if 41 >= 80
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao5_boss.mac
				endif
			endcase
			case 6
				if 41 < 50
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao1_boss.mac
				endif
				if 41 >= 50 and 41 < 60
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao2_boss.mac
				endif
				if 41 >= 60 and 41 < 70
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao3_boss.mac
				endif
				if 41 >= 70 and 41 < 80
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao4_boss.mac
				endif
				if 41 >= 80
					NEW_NPC_PRIVATE 2 0 333033 60 0 0 0 cangbao5_boss.mac
				endif
			
			endcase
		endswitch


		add_sys_msg_start
		add_sys_msg 经过仔细挖掘，你发现了一个秘密入口
		set_sys_msg_mode 4 0 0
		msg_info
		endif

		if @50 > 65
		
		if @41 >= 20 and @41 < 30
		NEW_LIFE_MON 280013 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 蝎子王从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 30 and @41 < 40
		NEW_LIFE_MON 280007 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 羽蛇神从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif


		
		if @41 >= 40 and @41 < 50
		NEW_LIFE_MON 280009 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 蝎子王从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 50 and @41 < 60
		NEW_LIFE_MON 280003 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 羽蛇神从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 60 and @41 < 70
		NEW_LIFE_MON 280010 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 蝎子王从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 70 and @41 < 80
		NEW_LIFE_MON 280004 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 羽蛇神从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 80 and @41 < 90
		NEW_LIFE_MON 280011 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 蝎子王从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 90 and @41 < 100
		NEW_LIFE_MON 280005 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 羽蛇神从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 100 and @41 < 110
		NEW_LIFE_MON 280012 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 蝎子王从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		if @41 >= 110 and @41 <= 120
		NEW_LIFE_MON 280006 1 @43 @44 500 0 0 0 1800 1 2
		add_sys_msg_start
		add_sys_msg 羽蛇神从地底爬出来守护自己的领地，看来只有击败他才能顺利拿到宝藏
		set_sys_msg_mode 4 0 0
		msg_info
		endif
		endif





		rand 34 10
		if @34 < 3 and @41 > 40
		;new_dyn_mon 1 225000 1 @43 @44 500 0 0 2 0 @41 0
		new_life_dyn_mon 1 225000 1 @43 @44 500 0 0 2 0 @41 0 60
		endif
		if @34 > 2 and @34 < 8 and @41 > 40
		;new_dyn_mon 1 225000 2 @43 @44 500 0 0 2 0 @41 0
		new_life_dyn_mon 1 225000 2 @43 @44 500 0 0 2 0 @41 0 60
		endif

		if @34 = 8 and @41 > 60
			WORLD_ORE_NEW_CHECK 295204 50
			if @50 = 0
				
					ADD_ITEM_NUM_INPAK 5012111 2 1 123
					ADD_ITEM_NUM_INPAK 5012110 5 1 123
					
					GET_MAP_INST 3110 1
					new_mon 295204 1 13610 13316 0 0 0 1 0
					add_sys_msg_start
					add_sys_msg $_player$开启藏宝图，使#L祈妙神坛@@(3110,13610,13316)#n处丰沥油田露出地面，获得大量的宝石奖励。
					;勤劳的$_player$成功开启藏宝图，引起地表运动，使富原矿坑在#L普罗圣地@@[18892,6876]#n刷新了
					set_sys_msg_mode 0 0 0
					SVR_MSG_INFO
					new_mon 255002 1 13255 12973 0 0 0 1 5
					TARGET_ADD_STATUS 3 @5 1095 1 72000 2
					new_mon 255003 1 13716 12980 0 0 0 1 2
					new_mon 255003 1 13192 13512 0 0 0 1 2
					GET_MAP_INST 0 1
			endif
		endif



		if @34 = 9 and @41 > 60
			WORLD_ORE_NEW_CHECK 295203 51
			if @51 = 0
				ADD_ITEM_NUM_INPAK 5012111 2 1 123
				ADD_ITEM_NUM_INPAK 5012110 5 1 123

					GET_MAP_INST 3110 1
					new_mon 295203 1 13667 5300 0 0 0 1 0
					
					add_sys_msg_start
					add_sys_msg $_player$开启藏宝图，使#L祈妙神坛@@(3110,13667,5300)#n处富原矿坑露出地面，获得大量的宝石奖励。
					;勤劳的$_player$成功开启藏宝图，引起地表运动，使丰沥油田在#L荫川林地@@[6073,14222]#n刷新了
					set_sys_msg_mode 0 0 0
					SVR_MSG_INFO
					new_mon 255000 1 13266 5783 0 0 0 1 5
					TARGET_ADD_STATUS 3 @5 1095 1 72000 2
					new_mon 255001 1 13228 5251 0 0 0 1 2
					new_mon 255001 1 13784 5894 0 0 0 1 2
					GET_MAP_INST 0 1
			endif
					
		endif

	endcase
	case 1
		add_sys_msg_start
		add_sys_msg 你确定没有看错？藏宝图描绘的地点似乎在越雾雨林地图
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 2
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 距离宝藏只有一步之遥,就在你的北方附近
		endcase
		case 2
		add_sys_msg 距离宝藏只有一步之遥,就在你的东北方附近
		endcase
		case 3
		add_sys_msg 距离宝藏只有一步之遥,就在你的东方附近
		endcase
		case 4
		add_sys_msg 距离宝藏只有一步之遥,就在你的东南方附近
		endcase
		case 5
		add_sys_msg 距离宝藏只有一步之遥,就在你的南方附近
		endcase
		case 6
		add_sys_msg 距离宝藏只有一步之遥,就在你的西南方附近
		endcase
		case 7
		add_sys_msg 距离宝藏只有一步之遥,就在你的西方附近
		endcase
		case 8
		add_sys_msg 距离宝藏只有一步之遥,就在你的西北方附近
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 3
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 宝藏不远了，就在你的北方
		endcase
		case 2
		add_sys_msg 宝藏不远了,就在你的东北方
		endcase
		case 3
		add_sys_msg 宝藏不远了,就在你的东方
		endcase
		case 4
		add_sys_msg 宝藏不远了,就在你的东南方
		endcase
		case 5
		add_sys_msg 宝藏不远了,就在你的南方
		endcase
		case 6
		add_sys_msg 宝藏不远了，就在你的西南方
		endcase
		case 7
		add_sys_msg 宝藏不远了,就在你的西方
		endcase
		case 8
		add_sys_msg 宝藏不远了,就在你的西北方
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 4
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg 宝藏距此处还有些距离,在你的北方
		endcase
		case 2
		add_sys_msg 宝藏距此处还有些距离,在你的东北方
		endcase
		case 3
		add_sys_msg 宝藏距此处还有些距离,在你的东方
		endcase
		case 4
		add_sys_msg 宝藏距此处还有些距离,在你的东南方
		endcase
		case 5
		add_sys_msg 宝藏距此处还有些距离,在你的南方
		endcase
		case 6
		add_sys_msg 宝藏距此处还有些距离,在你的西南方
		endcase
		case 7
		add_sys_msg 宝藏距此处还有些距离,在你的西方
		endcase
		case 8
		add_sys_msg 宝藏距此处还有些距离,在你的西北方
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	endswitch

endproc	