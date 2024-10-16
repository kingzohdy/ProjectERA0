;绿洲动态复活旗
proc 2

	strvar x y
	GET_MON_INFO 0 0 0 0 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\9535_16.ent")



	add_sys_msg_start
	strvar camp
	GET_ATTACKER_CAMP 1 camp
	if camp = 1
		add_sys_msg 绿洲方玩家$_Player$突袭占领了绿色原野复活点。
	elseif camp = 2
		add_sys_msg 沙漠方玩家$_Player$突袭占领了绿色原野复活点。
	else
		add_sys_msg 未知方玩家$_Player$突袭占领了绿色原野复活点。
	endif
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO
	
	SET_REVIVE_POS_CAMP 1 0 @camp
	NEW_DYN_MON 1 350046 1 16011 12511 0 0 0 2 @camp 0 0

	;SET_REVIVE_POS_CAMP #返回值 #复活点索引 #阵营索引
	



endproc
	