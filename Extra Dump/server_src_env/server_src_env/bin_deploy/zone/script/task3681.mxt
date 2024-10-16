
proc 1

	var 10 0
	set_task_var 3681 0 8 @10 1
	var 11 0
	set_task_var 3681 1 8 @11 1
	var 12 0
	set_task_var 3681 2 8 @12 1
	var 13 0
	set_task_var 3681 3 8 @13 1
	var 14 0
	set_task_var 3681 4 8 @14 1
	var 15 0
	set_task_var 3681 5 8 @15 1
	var 16 0
	set_task_var 3681 6 8 @16 1
	var 17 0
	set_task_var 3681 7 8 @17 1
	var 18 0
	set_task_var 3681 8 8 @18 1
	var 19 0
	set_task_var 3681 9 8 @19 1

	;失败次数
	var 20 0
	set_task_var 3681 10 8 @20 1
	
	;提示1次
	var 21 0
	set_task_var 3681 11 8 @21 1
	
	;提示锁1
	var 22 0
	set_task_var 3681 12 8 @22 1	
	;提示锁2
	var 23 0
	set_task_var 3681 13 8 @23 1	
	;提示锁3
	var 24 0
	set_task_var 3681 14 8 @24 1	
	;提示锁4
	var 25 0
	set_task_var 3681 15 8 @25 1	
	;提示锁5
	var 26 0
	set_task_var 3681 16 8 @26 1	
	;提示锁6
	var 27 0
	set_task_var 3681 17 8 @27 1	
	;提示锁7
	var 28 0
	set_task_var 3681 18 8 @28 1
	
	
	;锁
	var 29 0
	set_task_var 3681 19 8 @29 1
endproc 
proc 2
;	ADD_BIND_ITEM_NUM_INPAK 5041017 1 1 0
endproc 
proc 3
	strvar muclan retclan
	GET_SPY_TARGET_CLAN_ID retclan muclan
	if retclan = 0
		ADD_TARGET_CLAN_STABLE 1 @muclan -30 0
	endif
;;;;;;;
	strvar clanID
	GET_ID_PLAYER_INFO 0 18 clanID	

;	add_sys_msg_start
;	add_sys_msg $_ClanName$公会玩家:$_player$,成功夺取"$_SpyTargetClanName$"公会的情报,"$_ClanName$"公会获得奖励,"$_SpyTargetClanName$"公会安定度降低！
;	set_sys_msg_mode 4 0 0
;	SVR_MSG_INFO 
	add_sys_msg_start
	add_sys_msg $_ClanName$公会玩家:$_player$,成功夺取"$_SpyTargetClanName$"公会的情报,"$_ClanName$"公会获得奖励,"$_SpyTargetClanName$"公会安定度降低！
	set_sys_msg_mode 16 0 0
	SVR_MSG_INFO 
	
	GET_ITEM_NUM_INPAK 5300953 1 31
	DEC_ITEM_NUM_INPAK 5300953 @31 1	
	
	
endproc 
proc 4
	GET_ITEM_NUM_INPAK 5041017 1 31
	DEC_ITEM_NUM_INPAK 5041017 @31 1
	GET_ITEM_NUM_INPAK 5300950 1 32
	DEC_ITEM_NUM_INPAK 5300950 @32 1
	GET_ITEM_NUM_INPAK 5300951 1 33
	DEC_ITEM_NUM_INPAK 5300951 @33 1
	GET_ITEM_NUM_INPAK 5300952 1 34
	DEC_ITEM_NUM_INPAK 5300952 @34 1
	GET_ITEM_NUM_INPAK 5300953 1 35
	DEC_ITEM_NUM_INPAK 5300953 @35 1
	
	
	
endproc 
proc 5
	GET_ITEM_NUM_INPAK 5300950 1 30
	DEC_ITEM_NUM_INPAK 5300950 @30 1
	GET_ITEM_NUM_INPAK 5041017 1 31
	DEC_ITEM_NUM_INPAK 5041017 @31 1
	GET_ITEM_NUM_INPAK 5300951 1 33
	DEC_ITEM_NUM_INPAK 5300951 @33 1
	GET_ITEM_NUM_INPAK 5300952 1 34
	DEC_ITEM_NUM_INPAK 5300952 @34 1
	GET_ITEM_NUM_INPAK 5300953 1 35
	DEC_ITEM_NUM_INPAK 5300953 @35 1

endproc 

proc 6
	strvar pos pret
	PLAYER_CLAN_POS_INFO pret 0 0 0 pos
	if pret = 0 and pos = 0
		
		TASK_FAIL 3681 1
	endif
	strvar map
;	GET_PLAYER_MAPINFO 0 map 0 0 0 
;	if map != 60000
;		TASK_FAIL 3681 1
	
;	endif
	strvar ret
	IS_CLAN_SPY_OPEN ret
	if ret = 0
		
		TASK_FAIL 3681 1
	endif
	
	strvar clanID
	GET_ID_PLAYER_INFO 0 18 clanID
	get_task_var 3681 11 8 1 21 
	if 21 = 0
		IS_IN_CLAN_SPY_TARGET_MAP 50
		if 50 = 1
			add_sys_msg_start
			add_sys_msg 公会成员$_player$已经潜入<$_SpyTargetClanName$>公会,夺取情报去了！
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
			
			var 21 1
			set_task_var 3681 11 8 @21 1 
		endif
	endif
	
	get_task_var 3681 19 8 1 29 

	if 29 = 0
		strvar num
		GET_ITEM_NUM_INPAK 5300950 1 num
		if num >= 3
			SET_PLAYER_SPY_TASK_STAGE_DONE 1
			dec_item_num_inpak 5300951 1 0
		;	dec_item_num_inpak 5300950 5 0
			ADD_BIND_ITEM_NUM_INPAK 5300953 1 1 10
			var 29 1
			set_task_var 3681 19 8 @29 1 
		endif
	endif
endproc 