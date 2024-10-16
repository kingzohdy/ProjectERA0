

proc 3
	strvar clanID
	GET_ID_PLAYER_INFO 0 18 clanID	

	add_sys_msg_start
	add_sys_msg $_ClanName$公会的玩家:$_player$,成功夺回情报,$_ClanName$公会获得奖励！
	set_sys_msg_mode 16 0 0
	SVR_MSG_INFO 


endproc 

proc 4

	GET_ITEM_NUM_INPAK 5300950 1 31
	DEC_ITEM_NUM_INPAK 5300950 @31 1
endproc 
proc 5

	GET_ITEM_NUM_INPAK 5300950 1 31
	DEC_ITEM_NUM_INPAK 5300950 @31 1
endproc 

proc 6
	strvar map
	GET_PLAYER_MAPINFO 0 map 0 0 0 
;	if map != 60000
;		strvar ret
;		is_in_my_clan_city ret
;		if ret = 0
;			TASK_FAIL 3682 1
;		endif
;	endif

	strvar ret
	IS_CLAN_SPY_OPEN ret
	if ret = 0
		
		TASK_FAIL 3682 1
	endif
endproc 