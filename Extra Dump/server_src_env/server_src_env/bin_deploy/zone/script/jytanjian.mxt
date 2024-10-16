
;探监

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b亲~,想去监狱看看吗,500一次,不晚点,不限时的哦!

	
		option 150 #探监
		
	talk
endproc 

proc 150
	
	strvar mid Moy
	DEC_MONEY_TEST 0 500 20 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 您背包中的银币不足
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		choose 2 151 152 前往监狱探监,需要支付500银币,是否确认?
	endif
endproc
proc 151
	DEC_MONEY 0 500 20 10	
	if 10 = 0
		
		GET_PLAYER_ID 1 30
		TRANSMIT_CAMERA @30 135 1000 30200 18300 0 0
	endif
endproc 
proc 152
	return
endproc 
