;70副本,70副本隐藏boss电梯下行
;70副本隐藏boss电梯下行
PROC 1
	get_pworld_var 44 32 1 21
	if 21 > 0
		player_jump 1 19499 33267
		;需要飞行路线
		;autofly xxx
	endif	
	
	
ENDPROC
