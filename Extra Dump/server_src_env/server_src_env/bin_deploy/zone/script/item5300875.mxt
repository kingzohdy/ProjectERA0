;公会任务


proc 1

	strvar Mid PlayMid Gid1 Gid2 pGid1 pGid2 Pos
	;获取目标ID
	GET_TARGET_INFO 1 1 Mid
	PLAYER_CLAN_POS_INFO 1 @Mid Gid1 Gid2 Pos
	
	get_player_id 1 PlayMid
	PLAYER_CLAN_POS_INFO 1 @PlayMid pGid1 pGid2 0

	if @Gid1 = @pGid1 and @Gid2 = @pGid2
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 不可以对同公会的玩家使用此物品！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	else
		if Pos = 0
			SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg 您选择的目标没有公会，无法使用此物品！
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif

	dec_item_num_inpak 5300875 1 10
	
	if 10 != -1
		add_item_num_inpak 5300876 1 1 0
	endif
endproc

proc 2

	strvar Mid PlayMid Gid1 Gid2 pGid1 pGid2 Pos
	;获取目标ID
	GET_TARGET_INFO 1 1 Mid
	PLAYER_CLAN_POS_INFO 1 @Mid Gid1 Gid2 Pos
	
	get_player_id 1 PlayMid
	PLAYER_CLAN_POS_INFO 1 @PlayMid pGid1 pGid2 0

	if @Gid1 = @pGid1 and @Gid2 = @pGid2
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg 不可以对同公会的玩家使用此物品！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	else
		if Pos = 0
			SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg 您选择的目标没有公会，无法使用此物品！
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif

endproc
