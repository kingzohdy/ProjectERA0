;幻境传送员

proc 1
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b无尽的幻境等待着勇士们的探索!
	add_talk #b#b
	add_talk 进入条件：
	add_talk #b#b1、队长等级≥20级,队伍人数不少于3人；
	add_talk #b#b2、队伍中全体成员处于时间城,并且没有处于交易,摆摊,切磋,离线等状态。
	add_talk #b#b#c005ebe每周前50次闯层，杀怪可额外获大量经验#n
	
		task_add_option 1502 1 150
		task_add_option 3138 1 151
		task_add_option 3139 1 152
	
	
	option 100 进入幻境
	option 200 加入幻境队伍
	option 300 传送到队长所在幻境
	option 600 查询【闯层次数】
	option 500 副本须知
;	option 400 查看BOSS刷新情况
	talk	
endproc

proc 150
	task_do_option 1502 1
endproc 
proc 151
	task_do_option 3138 1
endproc  
proc 152
	task_do_option 3139 1
endproc 

;进入幻境
proc 100
	OPEN_UI 63
endproc

;加入幻境队伍
proc 200
	unreal_team_info 0
endproc

;传送到队长所在幻境
proc 300
	goto_unreal_captain_near 0
endproc

;查看BOSS刷新情况
proc 400
	unreal_watch_boss_info 0
endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk 副本须知：
	add_talk #b#b1、进入副本,消灭怪物后,确保最少有3名队员处于正常状态,如处于离线,死亡等状态,将无法传送到下一层；
	add_talk #b#b2、从幻境第10层开始,每五层会有一个小BOSS,有几率获得稀有的装备和宝物.
	option 1 返回
	talk
endproc
proc 2
	get_trig_task 1 2
	if 2 = 1502
		task_fini 1502 1
	endif
endproc 

proc 600
	DIALOG 1
	add_talk 闯层次数：
	add_talk 1. 幻境每周#c186506前50次#n闯层，杀怪额外获大量经验
	add_talk 2. 每闯上幻境1层，闯层次数#c186506减少1次#n
	add_talk 3. 本周#c186506剩余次数的一半#n，将积攒到下周的次数中
	add_talk 4 #c186506每周1#n将刷新本周的闯层次数
	add_talk 5. 闯层次数最高可积攒到#c186506100次#n
	add_talk #b#b
	strvar ret cishu
	UNREAL_GET_EXP_LAYER ret cishu
	if ret = 0 and cishu = 0
		add_talk 本周剩余闯层次数：#cF80B0B0#n次
	elseif ret = 0 and cishu > 0
		add_talk 本周剩余闯层次数：#c005ebe$@cishu$#n次
	else
	endif
	add_talk #b#b
	add_talk #c005ebe注意：每次进入或离开幻境，将减少1次闯层次数#n
	option 1 返回
	talk
endproc
