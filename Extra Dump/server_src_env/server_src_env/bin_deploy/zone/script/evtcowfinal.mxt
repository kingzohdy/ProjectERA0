;牧牛决赛,决赛事件
proc 1
;	MAP_NEW_NPC #返回标志 #NpcID #x #y #dir #map $脚本 
;	MAP_NEW_NPC 1 ::taskstartnpcid:: 18332 33387 ::taskstartnpcdir:: ::mapid:: npccowfinalstart.mac 
;	创建副本
	COW_PWORLD_CREATE 1
	if 1 = 0
		COW_FINAL_BEGIN 1
		add_sys_msg_start
		add_sys_msg 牛仔很忙决赛现在开始，请携带星光大道入场券前往彩虹岛，通过浩东进入星光大道，参与决赛！决赛前十名拥有非常丰厚的奖励哟！  
		set_sys_msg_mode 1 0 0
		svr_msg_info
	endif
endproc
proc 2
;	get_map_inst ::mapid:: 1
;	if 1 = 0
;		del_npc ::taskstartnpcid:: 1
;	endif
;	决赛结束
	COW_FINAL_END 1
	add_sys_msg_start
	add_sys_msg 牛仔很忙决赛到此结束，请各位参赛选手前往彩虹岛颁奖嘉宾-佳男处查询名次，领取奖励！  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
