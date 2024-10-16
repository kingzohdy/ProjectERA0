PROC 1

	strvar LV
	GET_PLAYER_BASE_ATT 0 0 LV 0
	if LV >= 10
		ADD_SYS_MSG_START
		SET_SYS_MSG_MODE 4 0 0
		ADD_SYS_MSG 全服智力竞赛已经开启,点击技能栏上方的智力竞赛活动助手即可参加,丰厚的奖励在等待着你!
		SVR_MSG_INFO
	endif
	
endproc 

