
proc 8
	TASK_STAT 4634 56 66
	if @56 = 0 and @66 = 2		
		strvar px py M
		GET_MAP_INST 1000 10
		GET_PLAYER_MAPINFO 1 1 1 px py
		GET_POINT_DIST 26391 38795 @px @py M

		if M <= 1000 and M >= 0		
			strvar MotIon
			ID_PLAYER_MOTION 0 0 MotIon     

			if MotIon = 10709
			;作揖
			    GET_MAP_INST 1000 10
				strvar npcmid
				GET_MAP_NPC_ID 306013 npcmid 1
				MON_SPEECH  @npcmid 0 0 1 国庆快乐，亲爱的，很高兴此时此刻能见到你！
				TASK_FINI 4634 0
			endif
		endif
	endif
endproc
