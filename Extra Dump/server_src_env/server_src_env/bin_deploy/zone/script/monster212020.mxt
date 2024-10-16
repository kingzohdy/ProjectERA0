;迷之塔里要保护的NPC 斯内瓦

;刷出怪物的事件
proc 1
	strvar hp
	GET_MON_INFO 1 0 10 hp 0 0 0 0
	
	;设置NPC初始血量
	;monsay10,NPC怪物受到攻击前血量
	set_mon_var 0 32 @hp 1
	
	var 11 1
	;monstate11,NPC怪物状态 0未刷 1存活 2死亡
	set_pworld_var 20 32 @11 1
endproc

;怪物死亡的事件
proc 2
	var 11 2
	;monstate11,NPC怪物状态 0未刷 1存活 2死亡
	set_pworld_var 20 32 @11 1
endproc

;怪物定时，3秒一次
proc 4
	strvar hp
	GET_MON_INFO 1 0 0 hp 0 0 0 0 
	get_mon_var 0 32 1 10
	
	;当前血量小于前一次进Tick时的血量
	if 10 > @hp
		;提示
		add_sys_msg_start
		add_sys_msg $_Monster$正受到怪物攻击，请尽快支援 
		set_sys_msg_mode 4 0 0
		map_msg_info

		;monhp11,枢纽怪上个tick的血量
		set_mon_var 0 32 @hp 1
	endif
	
endproc
