;创建事件
PROC 1
	strvar bossA
	var bossA 294050
	var 11 135
	var 12 45
	new_mon_by_dir @bossA 1 22821 12815 0 0 0 @11 1 2
	new_mon_by_dir @bossA 1 3559 12803 0 0 0 @12 1 2

	add_sys_msg_start
	add_sys_msg 战斗1分钟后开始，倒计时结束时如果有一方阵营不足3人，战场将关闭！
	set_sys_msg_mode 1 0 0
	map_msg_info
	
	;设置副本变量。10计时数
	var 10 0
	set_pworld_var 0 32 @10 1

	;;退出战场
	strvar TimC
	var TimC 0
	set_pworld_var 20 32 @TimC 1
	;退出战场2
	strvar TimLock
	var TimLock 5
	set_pworld_var 24 32 @TimLock 1
	;退出战场3
	strvar TimPover
	var TimPover 0
	set_pworld_var 28 32 @TimPover 1	
ENDPROC

;关闭事件
proc 2
endproc

;2秒一次
PROC 3
	get_pworld_var 0 32 1 10
	if 10 < 0
		return
	endif
	ADD 10 1
	set_pworld_var 0 32 @10 1

	;一分钟后,删阻挡,战斗开始
	if 10 = 30
		call 150
		
		;删阻挡
		delete_mon_by_defid 294050 1
		add_sys_msg_start
		add_sys_msg 进击吧！少年！推动粒子控制器摧毁敌方要塞！
		set_sys_msg_mode 3 0 0
		map_msg_info
		return
	endif
	
	if 10 > 30
		strvar Pnum
		GET_PWORLD_ROLE_NUM Pnum 1
		;战场没人时关闭
		if Pnum = 0
			pworld_over
			COMPEL_CLOSE_PWORLD 1			
		endif
		get_pworld_var 20 32 1 TimC 
		if TimC = 2
		;战场一方没人时开始5分钟计时
		
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;5分钟计时过程中，有人进入则中断倒计时	
			BATTLE_MIN_CAMP_PLAYER_NUM 1 num	
			if num != 0
				var TimC 0
				set_pworld_var 20 32 @TimC 1
				add_sys_msg_start
				add_sys_msg 战场中有援兵加入，战场关闭倒计时暂停，继续战斗吧！
				set_sys_msg_mode 1 0 0
				map_msg_info
				return
			endif
			
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1分钟之内，每10秒提示一次
			get_pworld_var 24 32 1 TimLock
			if TimLock = 1
				var 30 @10
				mod 30 5
				if 30 = 0
					get_pworld_var 28 32 1 TimPover
					add TimPover 10
					set_pworld_var 28 32 @TimPover 1
					var 50 60
					sub 50 @TimPover
					if 50 <= 0
						pworld_over
						COMPEL_CLOSE_PWORLD 1	
					endif
					
					add_sys_msg_start
					add_sys_msg 由于敌方阵营角色全部退出战场，$@50$秒后战场将会关闭！
					set_sys_msg_mode 4 0 0
					map_msg_info
					
				endif
				return
			endif
		
			var 15 @10
			mod 15 30
			if 15 = 0
				sub TimLock 1
				set_pworld_var 24 32 @TimLock 1
				
				add_sys_msg_start
				add_sys_msg 由于敌方阵营角色全部退出战场，$@TimLock$分钟后战场将会关闭！
				set_sys_msg_mode 4 0 0
				map_msg_info
			endif	
			return
		endif
		
		;战场一方没人时5分钟后关闭，每分钟输出一次
		BATTLE_MIN_CAMP_PLAYER_NUM 1 num	
		if num = 0
			add_sys_msg_start
			add_sys_msg 由于敌方阵营角色全部退出战场，5分钟后战场将会关闭！
			set_sys_msg_mode 4 0 0
			map_msg_info
			
			get_pworld_var 20 32 1 TimC
			var TimC 2
			set_pworld_var 20 32 @TimC 1
			
		endif
		return
	endif

	var 11 30
	sub 11 @10

	mod 10 5
	if 10 = 0
		mul 11 2
		add_sys_msg_start
		add_sys_msg 战斗$@11$秒后开始，倒计时结束时如果有一方阵营不足3人，战场将关闭！
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
ENDPROC

PROC 5
	;角色进入副本
	add_sys_msg_start
	add_sys_msg 在地图中寻找能源结晶，采集后可驱动中央的粒子控制器前进
	set_sys_msg_mode 3 0 0
	MSG_INFO
	
	EFFECT 2 1 SetMessageTips(210)
ENDPROC


PROC 150
	strvar num
	BATTLE_MIN_CAMP_PLAYER_NUM 1 num
	if num < 3
		add_sys_msg_start
		add_sys_msg 由于一方阵营不足3人,战场关闭，请重新报名！
		set_sys_msg_mode 4 0 0
		map_msg_info

		pworld_over
		COMPEL_CLOSE_PWORLD 1
		return
	endif
ENDPROC
