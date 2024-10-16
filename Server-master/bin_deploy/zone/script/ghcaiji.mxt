

proc 1

	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b想给我点好处吗？我可不容易被收买的……
	
	else	
	
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 1	
		add_talk $_npc$:
		add_talk #b#b您好，我是为公会收集资源的艾莉亚！
		add_talk #b#b
		add_talk #b#b#c186506如果你有资源和资金可以捐赠给我，我会代表公会感谢你的，并且也会给你一些好处哦#n
		add_talk #b#b#c186506#n
		add_talk #b#b
		add_talk #b#b#c8a8a8a小提示：公会资源的上限与公会等级相关联。公会储存的资源达到上限后，还是可以提交资源获得贡献度的#n
		if Ret = 100 or Ret = 0
;		add_talk #b#b#c186506当前采集中心等级为：0级#n		
		endif
		if Ret = 101 or Ret = 1
;		add_talk #b#b#c186506当前采集中心等级为：1级#n
		endif
		if Ret = 102 or Ret = 2
;		add_talk #b#b#c186506当前采集中心等级为：2级#n
		endif
		strvar CRet
		GET_CLAN_USED_TECH_POINT CRet 0 2
		
		if Ret > 0 and Ret != 100
		
;			option 120 #进入采集场
			if CRet > 0
;				option 121 #采集车制造			
			endif
			if Ret < 2
			
;				option 119 #升级采集中心
			endif
			
		endif
;		option 200 #采集车提取资源
		option 201 #向公会捐赠资源
		
		
		option 298 #向公会捐赠资金
		option 400 #资金捐赠榜
		
	endif
		
	talk
endproc 
proc 200
	LAYDOWN_CLAN_ORE 0 0
endproc


proc 201
	
	PLAYER_CLOSE_DIALOG 0 0
	DIALOG 3
	add_talk $_npc$:
	add_talk #b#b做为公会的一员，有责任和义务给公会捐赠资源！当然我也会给你好处的……
	add_talk 可捐赠资源：
	add_talk #b#b矿类：原质矿、密英矿、热能矿、乌损矿、苍恒矿
	add_talk #b#b油类：原生油、低构油、光合油、晶脉油、澈星油


	add_talk #b#b
		SET_DLG_COMMIT_NUM 5 10
		NPC_COMMIT_OPT 202
	talk
endproc
proc 298
		PLAYER_CLOSE_DIALOG 0 0
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DIALOG 1
	add_talk $_npc$:
	add_talk 捐赠资金规则：
	add_talk #b#b捐赠1000 #c186506银币#n，公会增加1000资金，个人获得10点贡献度！
	add_talk #b#b捐赠10000 #c186506银币#n，公会增加10000资金，个人获得100点贡献度！
	add_talk #b#b捐赠50000 #c186506银币#n，公会增加50000资金，个人获得500点贡献度！
	add_talk #b#b
;	add_talk #b#b#c186506<提示：当绑银不足时，会自动扣除银币>#n
	
		option 290 捐赠1000公会资金
		option 291 捐赠10000公会资金
		option 292 捐赠50000公会资金
	talk
endproc 
proc 290
	choose 2 310 305 确定捐赠1000银币作为公会资金？
endproc
proc 291
	choose 2 320 305 确定捐赠10000银币作为公会资金？
endproc
proc 292
	choose 2 330 305 确定捐赠50000银币作为公会资金？
endproc

proc 300
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 1000 and @ret >= 1000
		; choose 1 310 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	; elseif ret < 1000
		; choose 1 310 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	; else
		; call 310
	; endif
endproc 

proc 299
	return
endproc

proc 310
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		return
	endif
	DEC_MONEY_TEST 0 1000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg 你没有那么多钱！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 1000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 12 0 1000 0 0
	;	ADD_ID_PLAYER_CONTRI_TEST 0 0 10 0	
		if 12 = 0
			ADD_ID_PLAYER_CONTRI 0 0 10 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 1000 1000 10
			
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg 公会成员$_player$向公会捐赠了1000资金，感谢他对公会的帮助！
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
		endif
	endif

endproc 
proc 301
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 10000 and @ret >= 10000
		; choose 1 320 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	; elseif ret < 10000
		; choose 1 320 299 您绑银不足，继续操作将直接消耗银币，是否继续操作？
	; else
		; call 320
	; endif
endproc 

proc 320
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DEC_MONEY_TEST 0 10000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg 你没有那么多钱！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 10000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 13 0 10000 0 0
		if 13 = 0
		;	ADD_ID_PLAYER_CONTRI_TEST 0 0 20 0	
			ADD_ID_PLAYER_CONTRI 0 0 100 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 10000 10000 100
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg 公会成员$_player$向公会捐赠了10000资金，感谢他对公会的帮助！
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
			
		endif
	endif

endproc 
proc 302
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 50000 and @ret >= 50000
		; choose 1 330 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	; elseif ret < 50000
		; choose 1 330 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	; else
		; call 330
	; endif
endproc 

proc 330
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DEC_MONEY_TEST 0 50000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg 你没有那么多钱！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 50000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 14 0 50000 0 0
	;	ADD_ID_PLAYER_CONTRI_TEST 0 0 20 0	
		if 14 = 0
			ADD_ID_PLAYER_CONTRI 0 0 500 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 50000 50000 500
			
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg 公会成员$_player$向公会捐赠了50000资金，感谢他对公会的帮助！
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
		endif
	endif

endproc 


proc 400
	open_ui 69
endproc 

proc 202
;	strvar Num 
;	GET_MUL_COMMIT_ITEM_INFO Ret 0 3 Num 
;	if Ret = 0
;		COMMIT_CLAN_ITEM 1 0 @Num		
;	endif
	COMMIT_MUL_CLAN_ITEM 0
	PLAYER_CLOSE_DIALOG 0 0
endproc


proc 119
	open_ui 22
endproc	

proc 120

	TRANSMIT 4000 17100 10800 1
endproc
proc 121
	open_ui 46
endproc 
proc 122
	OPEN_NPC_SHOP 
endproc

