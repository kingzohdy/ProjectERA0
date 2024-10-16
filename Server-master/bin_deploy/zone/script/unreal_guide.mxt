;幻境NPC向导
proc 1
	is_captain 10 0
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b下一个未知的世界就在眼前,你准备好了吗?
	;判断角色是否是队长
	if @10 = 0
		get_unreal_layer 1 20
		if @20 < 110
		option 100 #前往下一层
		endif
		var 30 @20
		MOD @30 10
		if @30 = 0 AND @20 >= 20
			DEV @20 10
			SUB @20 1
			SWITCH @20
				CASE 1
			;		option 200 #前往BOSS地图 - 傲慢之境
				endcase
				CASE 2
			;		option 200 #前往BOSS地图 - 贪婪之境
				endcase
				CASE 3
			;		option 200 #前往BOSS地图 - 纵欲之境
				endcase
				CASE 4
			;		option 200 #前往BOSS地图 - 暴怒之境
				endcase
				CASE 5
			;		option 200 #前往BOSS地图 - 逸乐之境
				endcase
				CASE 6
			;		option 200 #前往BOSS地图 - 妒忌之境
				endcase
				CASE 7
			;		option 200 #前往BOSS地图 - 怠惰之境
				endcase
				CASE 8
			;		option 200 #前往BOSS地图 - 欺诈之境
				endcase
				CASE 9
			;		option 200 #前往BOSS地图 - 堕落之境
				endcase
				CASE 10
			;		option 200 #前往BOSS地图 - 原罪之境
				endcase
			ENDSWITCH
		endif
	endif
	
	option 300 #返回时间城
	talk	
endproc 

proc 100
	strvar Maxlv
	var Maxlv 0
	
	strvar Layer
	var Layer 0
	
	strvar Rflag
	var Rflag 0

	is_captain 10 0
	if @10 = 0
		GET_CUR_MAX_LEVEL Rflag Maxlv
		get_unreal_layer 1 Layer
		
		if Maxlv >= 80
			SUB Maxlv 5
			
			if @Rflag = 0 and @Maxlv > @Layer
				UNREAL_ENTER 0 1
			else
				add_sys_msg_start
				add_sys_msg	下一层幻境将在等级封印解除后开放
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		elseif Maxlv < 80
			SUB Maxlv 10
			
			if @Rflag = 0 and @Maxlv > @Layer
				UNREAL_ENTER 0 1
			else
				add_sys_msg_start
				add_sys_msg	下一层幻境将在等级封印解除后开放
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
		endif
	else
		add_sys_msg_start
		add_sys_msg	可惜了，您现在已不是队长，无法将队伍传入下一层了。
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 200
	UNREAL_ENTER_BOSS_CHECK 1
	if @1 = -1
		return
	endif
	choose 0 210 220 　　此处魔王携带大量宝藏，将会遭遇多个冒险的玩家队伍，PK无惩罚，请谨慎进入！
endproc

proc 210
	UNREAL_ENTER_BOSS_CHECK 1
	if @1 = -1
		return
	endif
	get_unreal_layer 1 10
		
	DEV @10 10
	SUB @10 1
	
	SWITCH @10
		CASE 1
			TEAM_TRANS_INSAMEMAPINST 2600 4732	3620 0
		endcase
		CASE 2
			TEAM_TRANS_INSAMEMAPINST 2601 4801	3716 0
		endcase
		CASE 3
			TEAM_TRANS_INSAMEMAPINST 2602 4732	3620 0
		endcase
		CASE 4
			TEAM_TRANS_INSAMEMAPINST 2603 4801	3716 0
		endcase
		CASE 5
			TEAM_TRANS_INSAMEMAPINST 2604 4732	3620 0
		endcase
		CASE 6
			TEAM_TRANS_INSAMEMAPINST 2605 4801	3716 0
		endcase
		CASE 7
			TEAM_TRANS_INSAMEMAPINST 2606 4732	3620 0
		endcase
		CASE 8
			TEAM_TRANS_INSAMEMAPINST 2607 4801	3716 0
		endcase
		CASE 9
			TEAM_TRANS_INSAMEMAPINST 2608 4732	3620 0
		endcase
		CASE 10
			TEAM_TRANS_INSAMEMAPINST 2609 4801	3716 0
		endcase
	ENDSWITCH
endproc

proc 220
	return
endproc

proc 300
	choose 0 301 302 确定返回时间城吗?
endproc

proc 301
	is_captain 20 0
	if @20 = 0
		TEAM_TRANS_INSAMEMAPINST 1000 40765 35339 0
	else
		TRANSMIT 1000 40765 35339 0
	endif
endproc

proc 302

endproc


