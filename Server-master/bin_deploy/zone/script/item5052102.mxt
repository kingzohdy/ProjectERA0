;;;中级探测器

proc 1

	strvar Locw Rad Num1 Num2 Num3 Num4 Num5
	
	GET_NPC_NUM 307015 1 Num1
	GET_NPC_NUM 307016 1 Num2
	GET_NPC_NUM 307017 1 Num3
	GET_NPC_NUM 307018 1 Num4
	GET_NPC_NUM 307019 1 Num5
	
	get_pworld_var 100 8 1 Locw	
	
	if Locw = 1
	
		strvar px py
		GET_PLAYER_MAPINFO 0 0 0 px py
		rand Rad 50	
		;挖出怪物
		if Rad > 0 and Rad < 3
			strvar LocMon ClanID
			var LocMon 10
			set_pworld_var 101 8 @LocMon 1
			set_pworld_var 12 32 @px 1
			set_pworld_var 16 32 @py 1
			
			GET_MON_NUM_RADIUS 0 @px @py 3000 284272 50 60
			if 50 = 0 and 60 = 0
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\1000900_8.ent")
				;记录挖出怪物的x y值
				add_sys_msg_start
				add_sys_msg 你惊动了宝藏守护者！
				set_sys_msg_mode 3 0 0
				msg_info	
				
				CALL_MON 1 284272 0 60 @px @py 1
				;得到公会ID
				get_pworld_var 0 32 1 ClanID	
				add_sys_msg_start
				add_sys_msg 运气真不好，【$_player$】惊动了宝藏守护者，守护者会阻止大家挖宝，引走怪物或许会好很多！
				set_sys_msg_mode 14 0 0
				CLAN_MSG_INFO 1 @ClanID
			else
				call 100
			endif
		endif

		;十分之一几率挖出宝箱
		if Rad = 0
		strvar dir radp
			rand dir 180
			rand radp 50
			add px @radp
			add py @radp
			if Num1 = 0
				NEW_LIFE_NPC 1 307015 @px @py @dir 21500 xbxiang_1.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			elseif Num2 = 0

				NEW_LIFE_NPC 1 307016 @px @py @dir 21500 xbxiang_2.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num3 = 0
				NEW_LIFE_NPC 1 307017 @px @py @dir 21500 xbxiang_3.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num4 = 0
				NEW_LIFE_NPC 1 307018 @px @py @dir 21500 xbxiang_4.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num5 = 0
				NEW_LIFE_NPC 1 307019 @px @py @dir 21500 xbxiang_5.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			else
				call 100
			endif
		endif
		
		;挖出物品
		if Rad >= 3 and Rad < 50
				;根据自身等级掉落
		;	strvar Locg
		;	get_pworld_var 64 8 1 Locg
		;	if Locg = 10
		;		call 200
		;	endif
		;	if Locg = 20
		;		call 201
		;	endif
		;	if Locg = 30
		;		call 202
		;	endif
			strvar lv
			GET_PLAYER_BASE_ATT 0 0 LV 0
			if lv < 40
				call 200
			endif
			if lv >= 40 and lv < 60
				call 201
			endif
			if lv >= 60 and lv < 70
				call 202
			endif
			if lv >= 70 and lv < 80
				call 205
			endif
			if lv >= 80 and lv < 100
				call 203
			endif
			if lv >= 100 and lv <= 120
				call 204
			endif
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 附近的宝藏已经挖空了，换个地方吧！
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc  

proc 2
		strvar Rets Mapc Plos
	PLAYER_CLAN_POS_INFO 0 0 0 0 Plos
	if Plos = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 先去加入个公会吧，你只能参加自己公会的活动才可以挖宝！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	DYN_ACTIVE_STATUS_GET  1 19 Rets
	if Rets = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 公会宝藏活动未开启，不能使用物品！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	
	strvar ret
	CLAN_JOIN_TIME_NO_SAMEDAY ret
	if ret = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 你好像还没熟悉公会城市呢，加入公会当天无法挖掘公会宝藏！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	
	
	GET_PLAYER_MAPINFO 1 Mapc 0 0 0 
	if Mapc != 60000
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 挖掘器只能在公会城市地图使用！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar ClanID PlayerID
		;记录的公会ID	
	;	get_pworld_var 0 32 1 ClanID	
		;得到人物的公会ID
	;	GET_ID_PLAYER_INFO 0 18 PlayerID
	;	if ClanID != @PlayerID
	IS_IN_MY_CLAN_CITY 20
	if 20 != 1
		SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg 当前地图不是你自己所在公会的城市，不能挖掘宝藏！
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar midx midy px py M
	;获取发现宝藏时飞碟的位置
	get_pworld_var 4 32 1 midx 
	get_pworld_var 8 32 1 midy 
	
	;使用物品时人物的位置
	GET_ID_PLAYER_INFO 0 3 px
	GET_ID_PLAYER_INFO 0 4 py
	;获得距离
	GET_POINT_DIST @midx @midy @px @py M
	
	strvar Locw
	get_pworld_var 100 8 1 Locw
	if Locw = 1
		if M > 800
			SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg 你所在的位置距离宝藏探测器有点远，不容易发现宝藏！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	else
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 探测器还没有发现宝藏，这样是挖不出宝藏的哦！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif	
endproc 

proc 100
	add_sys_msg_start
	add_sys_msg 这次什么也没有挖到，继续努力！
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 101

	add_sys_msg_start
	add_sys_msg 背包里的东西太多了，先清理一下背包吧！
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 300
	;得到公会ID
	get_pworld_var 0 32 1 ClanID	
	add_sys_msg_start
	add_sys_msg 运气真好，【$_player$】居然挖到了大宝箱，大家快去看看宝箱里有什么！
	set_sys_msg_mode 14 0 0
	CLAN_MSG_INFO 1 @ClanID
	
endproc 

proc 200
;1~40级
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 100
	;if Radn = 0
	;	ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	;endif
	;if Radn = 1
	;	ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	;endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;灵魂契约
	;endif
	if Radn >= 0 and Radn < 3
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if Radn >= 3 and Radn < 6
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;旷岩宝珠
	endif
;	if Radn >= 6 and Radn < 8
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;无瑕宝石升级符
;	endif
;	if Radn >= 8 and Radn < 10
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;古老藏宝图
;	endif
	
	if Radn >= 6 and Radn < 12
		ADD_ITEM_NUM_INPAK 5060064 1 1 0
		;生存奖章*3
	endif

	if Radn >= 12 and Radn < 41
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;秘境精华
	endif
	if Radn >= 41 and Radn < 71
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;原质矿
	endif
	if Radn >= 71 and Radn < 100
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;原生油
	endif
	
endproc 

proc 201
; 40 ~60级
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 1000
	if Radn >= 0 and Radn < 3
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	endif
	if Radn >= 3 and Radn < 5
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;灵魂契约
	;endif
	if Radn >= 5 and Radn < 30
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if Radn >= 30 and Radn < 40
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;旷岩宝珠
	endif
	if Radn >= 40 and Radn < 50
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;熔火宝珠
	endif
;	if Radn >= 50 and Radn < 60
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;无瑕宝石升级符
;	endif
;	if Radn >= 60 and Radn < 80
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;古老藏宝图
;	endif

	if Radn >= 50 and Radn < 110
		ADD_ITEM_NUM_INPAK 5060065 1 1 0
		;生存奖章*5
	endif

	if Radn >= 110 and Radn < 200
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;秘境精华
	endif
	if Radn >= 200 and Radn < 410
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;宁静精华
	endif
	if Radn >= 410 and Radn < 510
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;原质矿
	endif
	if Radn >= 510 and Radn < 710
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;密英矿
	endif
	if Radn >= 710 and Radn < 810
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;原生油
	endif
	if Radn >= 810 and Radn < 1000
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;低构油
	endif
endproc 

proc 202
;60-70
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 1000
	if Radn >= 0 and Radn < 3
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	endif
	if Radn >= 3 and Radn < 5
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;灵魂契约
	;endif
	if Radn >= 5 and Radn < 30
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if Radn >= 30 and Radn < 40
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;旷岩宝珠
	endif
	if Radn >= 40 and Radn < 50
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;熔火宝珠
	endif
;	if Radn >= 50 and Radn < 60
;	;	ADD_ITEM_NUM_INPAK 5109203 1 1 0
;		ADD_ITEM_NUM_INPAK 5103001 1 1 0
;		;狂澜改成秘境净化
;	endif
;	if Radn >= 60 and Radn < 80
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;无瑕宝石升级符
;	endif
;	if Radn >= 80 and Radn < 110
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;古老藏宝图
;	endif
	
	if Radn >= 50 and Radn < 110
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;生存奖章*10
	endif

	if Radn >= 110 and Radn < 160
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;秘境精华
	endif
	if Radn >= 160 and Radn < 220
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;宁静精华
	endif
	if Radn >= 220 and Radn < 410
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
		;冷川精华
	endif
	if Radn >= 410 and Radn < 450
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;原质矿
	endif
	if Radn >= 450 and Radn < 510
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;密英矿
	endif
	if Radn >= 510 and Radn < 710
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
		;热能矿
	endif
	if Radn >= 710 and Radn < 750
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;原质油
	endif
	if Radn >= 750 and Radn < 810
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;低构油
	endif
	if Radn >= 810 and Radn < 1000
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
		;光合油
	endif
endproc

 
proc 203
	;80-100
	strvar Num rad
	GET_PACK_SPACE 0 Num
		
	if Num = 0
		call 101
		return
	endif
	rand rad 100
	if rad = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	endif
	if rad = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	endif
;	if rad = 2
;		ADD_ITEM_NUM_INPAK 5050510 1 1 0
;		;灵魂契约
;	endif
	if rad = 2
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if rad = 3
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;熔火宝珠
	endif
	if rad = 4
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
	endif
	if rad = 5
		ADD_ITEM_NUM_INPAK 5109204 1 1 0
	endif 
;	if rad >= 6 and rad < 9
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;无瑕宝石升级符
;	endif
;	if rad >= 9 and rad < 16
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;古老藏宝图
;	endif
	if rad >= 6 and rad < 12
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;生存奖章*10
	endif

	if rad >= 12 and rad < 20
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;宁静精华
	endif
	if rad >= 20 and rad < 26
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if rad >= 26 and rad < 44
		ADD_ITEM_NUM_INPAK 5103004 1 1 0
	endif
	if rad >= 44 and rad < 48
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;密英矿
	endif
	if rad >= 48 and rad < 54
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
	endif
	if rad >= 54 and rad < 72
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
	if rad >= 72 and rad < 76
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;低构油
	endif
	if rad >= 76 and rad < 82
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif
	if rad >= 82 and rad < 100
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
endproc 

proc 204
	;100-120
	strvar Num rad
	GET_PACK_SPACE 0 Num
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 这么高级的探测器还没研发成功，这样是挖不出宝藏的哦！
		set_sys_msg_mode 3 0 0
		msg_info
		return
		;暂时不可用
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	if Num = 0
		call 101
		return
	endif
	rand rad 100
	if rad = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	endif
	if rad = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	endif
	if rad = 2
		ADD_ITEM_NUM_INPAK 5050510 1 1 0
		;灵魂契约
	endif
	if rad = 3
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if rad = 4
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
	endif
	if rad = 5
		ADD_ITEM_NUM_INPAK 5109204 1 1 0
	endif
	if rad = 6
		ADD_ITEM_NUM_INPAK 5109205 1 1 0
	endif 
	if rad >= 7 and rad < 9
		ADD_ITEM_NUM_INPAK 5219400 1 1 0
		;无瑕宝石升级符
	endif
	if rad >= 9 and rad < 16
		ADD_ITEM_NUM_INPAK 5062000 1 1 0
		;古老藏宝图
	endif
	if rad >= 16 and rad < 20
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if rad >= 20 and rad < 26
		ADD_ITEM_NUM_INPAK 5103004 1 1 0
	endif
	if rad >= 26 and rad < 44
		ADD_ITEM_NUM_INPAK 5103005 1 1 0
	endif
	if rad >= 44 and rad < 48
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
	endif
	if rad >= 48 and rad < 54
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
	if rad >= 54 and rad < 72
		ADD_ITEM_NUM_INPAK 5101005 2 1 0
	endif
	if rad >= 72 and rad < 76
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif
	if rad >= 76 and rad < 82
		ADD_ITEM_NUM_INPAK 5102004 2 1 0
	endif
	if rad >= 82 and rad < 100
		ADD_ITEM_NUM_INPAK 5102005 2 1 0
	endif
endproc 



proc 205
;70-80
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 100
	if Radn = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;幻化果实
	endif
	if Radn = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;基因果实
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;灵魂契约
	;endif
	if Radn = 2
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;宝石碎片
	endif
	if Radn = 3
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;旷岩宝珠
	endif
	if Radn = 4
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;熔火宝珠
	endif
	if Radn = 5
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
		;狂澜
	endif
;	if Radn >= 6 and Radn < 8
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;无瑕宝石升级符
;	endif
;	if Radn >= 8 and Radn < 12
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;古老藏宝图
;	endif
	
	if Radn >= 6 and Radn < 12
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;生存奖章*10
	endif		
	
	if Radn >= 12 and Radn < 16
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;秘境精华
	endif
	
	if Radn >= 16 and Radn < 22
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;宁静精华
	endif
	if Radn >= 22 and Radn < 41
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if Radn >= 41 and Radn < 45
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;原质矿
	endif
	if Radn >= 45 and Radn < 51
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
	endif
	if Radn >= 51 and Radn < 71
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
		;热能矿
	endif
	if Radn >= 71 and Radn < 75
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;原生油
	endif
	if Radn >= 75 and Radn < 81
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;低构油
	endif
	if Radn >= 81 and Radn < 100
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif

endproc


