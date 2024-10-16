;转职副本逻辑区域


proc 1

	get_pworld_var 7 8 1 53
	if 53 = 0

		strvar mX mY Mid
		GET_ID_PLAYER_INFO 0 3 mX
		GET_ID_PLAYER_INFO 0 4 mY
		add mX 100
		add mY 100
			new_npc 1 304041 @mX @mY 0 zhleimenglinghun.mac
			
			GET_MAP_NPC_ID 304041 Mid 1
			TARGET_ADD_STATUS 2 @Mid 1257 1 7200 1
			EFFECT_ID 0 @Mid playMontion(%d,"1204001")
			mon_speech @Mid 2 2 1 年轻人，不要害怕，我会帮助你完成这段旅程。
			
		var 53 1
		set_pworld_var 7 8 @53 1
		
		strvar Tim1
		var Tim1 1
		set_pworld_var 8 8 @Tim1 1
	endif
endproc 

