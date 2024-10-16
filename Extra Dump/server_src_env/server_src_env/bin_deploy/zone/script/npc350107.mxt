;跨服公会战副本    ,飞行器
proc 1
	;battlestat30,战斗阶段|0初始阶段 1准备阶段 2开始阶段
	var 30 0
	get_pworld_var 80 32 1 30
	if 30 != 2
		return
	endif
	autofly 126 1
	DEL_NPC 0 1
	
endproc
