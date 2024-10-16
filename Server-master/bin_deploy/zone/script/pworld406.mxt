;创建事件
PROC 1					
	
	
	;种2个boss
	;中立为0,绿洲为1,沙漠为2
	;女boss,绿洲
	strvar bossA
	strvar bossACamp
	strvar minLevel maxLevel dynLevel
	GET_BATTLE_LEVEL_LIMIT 1 minLevel maxLevel
	var dynLevel @maxLevel
	sub dynLevel @minLevel
	dev dynLevel 2
	add dynLevel 1
	add dynLevel @minLevel

	var bossA 282071
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 4711 4311 0 0 0 2 @bossACamp @dynLevel 1

	;男boss,沙漠
	var bossA 282070
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 25911 21311 0 0 0 2 @bossACamp @dynLevel 1

	;种4个阻挡
	call 100
	
	;种2个动态复活点

	var bossA 350046
	;var bossA 350046
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 16011 12511 0 0 0 2 @bossACamp 0 0

	var bossA 350048
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 21111 6811 0 0 0 2 @bossACamp 0 0

	;一个万磁干扰器的旗帜

	var bossA 350050
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 17277 8574 0 0 0 2 @bossACamp 0 0

	;绿洲飞行器,带干扰特效
	;NEW_CAMP_NPC #返回值 #NpcID #坐标x #坐标y #方向 #阵营索引 #脚本
	;NEW_CAMP_NPC 1 350061 11607 6789 0 1 battleoasisfly.mac

	;沙漠飞行器,带干扰特效
	;NEW_CAMP_NPC #返回值 #NpcID #坐标x #坐标y #方向 #阵营索引 #脚本
	;NEW_CAMP_NPC 1 350062 17755 18901 0 2 battledesertfly.mac

	;绿洲传送球
	;NEW_CAMP_NPC 1 333038 5929 2851 0 1 battleoasisout.mac
	;沙漠传送球
	;NEW_CAMP_NPC 1 333039 26107 21261 0 2 battledesertout.mac

	add_sys_msg_start
	add_sys_msg 战斗1分钟后开始，倒计时结束时如果有一方阵营不足3人，战场将关闭！
	set_sys_msg_mode 1 0 0
	map_msg_info
	
	;设置副本变量。10计时数
	var 10 0
	set_pworld_var 0 32 @10 1
	;11万磁干扰器的阵营,默认0中立
	var 11 0
	set_pworld_var 4 32 @11 1
	;绿洲女boss计时
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;沙漠男boss计时
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;lock1分钟后开始掉血
	strvar Lock
	var Lock 0
	set_pworld_var 16 32 @Lock 1
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
	;记录当前副本级别
	strvar pworld_lv
	var pworld_lv 406
	set_pworld_var 32 32 @pworld_lv 1
ENDPROC

;关闭事件
proc 2
	add_sys_msg_start
	add_sys_msg 战斗结束
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc

;2秒一次
PROC 3
	get_pworld_var 0 32 1 10
	if 10 < 0
		return
	endif
	ADD 10 1
	set_pworld_var 0 32 @10 1
	strvar num
	;一分钟后,删阻挡,战斗开始
	if 10 = 30
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
		;删阻挡
		delete_mon_by_defid 294008 1
		;控制开始掉血的变量
		get_pworld_var 16 32 1 Lock 1
		var Lock 10
		set_pworld_var 16 32 @Lock 1
		add_sys_msg_start
		add_sys_msg 战斗开始，指挥官会受到敌方空袭伤害，占领干扰器可干扰敌方空袭的飞行器
		set_sys_msg_mode 4 0 0
		map_msg_info
		return
	endif
	if 10 > 30
		var 40 @10
		mod 40 30
		if 40 = 0
			add_sys_msg_start
			add_sys_msg 占领地图中央干扰器可使己方阵营指挥官免于空袭！
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
		if Num != 0
			var TimC 0
			set_pworld_var 20 32 @TimC 1
			add_sys_msg_start
			add_sys_msg 战场中有援兵加入，战场关闭倒计时暂停，继续战斗吧！
			set_sys_msg_mode 1 0 0
			map_msg_info
			return
		endif
			
		;;;;;5;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1分钟之内，每10秒提示一次
	
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
		;战场一方没人时5分钟后关闭
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
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	
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
	add_sys_msg 击杀对方阵营指挥官即可获得胜利
	set_sys_msg_mode 7 0 0
	msg_info
	
	EFFECT 2 1 SetMessageTips(200)

ENDPROC

proc 100
	;绿洲
	;NEW_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #返回标志 #第一只怪物的实例ID
	;NEW_MON 230014 4 7003 3532 0 0 0 1 2
	var 10 49
	new_mon_by_dir 294008 1 6464 3871 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 6410 3532 0 0 0 @10 1 2
	
	var 10 0
	new_mon_by_dir 294008 1 5194 6413 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 4822 6435 0 0 0 @10 1 2 
	new_mon_by_dir 294008 1 5004 6437 0 0 0 @10 1 2

	;沙漠
	var 10 0
	new_mon_by_dir 294008 1 24949 17835 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25306 17840 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25694 17867 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25504 17850 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25169 17859 0 0 0 @10 1 2
	;new_mon_by_dir 294008 1 24718 17832 0 0 0 @10 1 2
	var 10 45
	new_mon_by_dir 294008 1 23435 20293 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 23436 19893 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 23442 20066 0 0 0 @10 1 2
endproc

;以下为调试用,可删除
;重置战场,调试用
PROC 10
	pworld_again
ENDPROC

;绿洲飞行器的脚本zcflyblue,
;---需要飞行路径
proc 11
	autofly 114 1
endproc

;沙漠飞行器的脚本zcflyred,暂时放这
;---需要飞行路径
proc 12
	autofly 115 1
endproc

;传送点脚本,outbattle
PROC 13
        choose 2 14 15 是否要离开当前战场？
ENDPROC

proc 14
        OUT_PWORLD_BY_SINGLE
endproc

proc 15

endproc

proc 16
	;女boss,绿洲
	strvar bossA
	strvar bossACamp
	var bossA 282071
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 25957 19875 0 0 0 2 @bossACamp 0 0

	;男boss,沙漠
	var bossA 282070
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 5838 5400 0 0 0 2 @bossACamp 0 0
endproc


proc 101
	delete_mon_by_defid 294008 1
endproc

proc 102
	var 10 45
	new_mon_by_dir 294008 1 5480 4687 0 0 0 @10 1 2
endproc

proc 103
		
	del_npc 350062 1
	del_npc 350061 1

	del_npc 350047 1
	del_npc 350049 1
endproc