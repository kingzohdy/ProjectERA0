


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b为解决近期资源严重短缺的问题，联盟决定以公会为单位实施“BOSS养成”计划！该计划主要内容如下：
	add_talk #b#b
	add_talk #b#b#c186506 1、公会达到3级自动领养BOSS 
	add_talk #b#b 2、召唤后3个小时未击杀，BOSS会消失
	add_talk #b#b 3、只有会长才有权限激活BOSS
	add_talk #b#b 4、每周日6点-24点之间可激活BOSS
	add_talk #b#b在此时间段内未手动激活则BOSS自动激活
	add_talk #b#b#c6d00d2击杀boss时，若公会建立、入会时间不足24小时，无法获得经验、公会资金奖励#n
	
	
	is_in_my_clan_city 50
	if 50 = 1
	
		task_add_option 3018 1 100
	
		option 150 “BOSS”养成计划详细介绍
;		option 200 #领养BOSS
	endif
	talk
	
endproc 
PROC 100
	task_do_option 3018 1
ENDPROC

proc 150
	dialog 1
	add_talk BOSS资质：
	add_talk #b#bBOSS分为普通、良好、优秀、杰出
	add_talk #b#b最初BOSS资质为普通，喂养食物可使BOSS的资质进阶，BOSS资质越高，击杀时的奖励越高！
	add_talk #b#b
	add_talk 领养限制：
	add_talk #b#b公会达到3级自动领养BOSS；BOSS被激活后（包括会长手动激活、时间到了之后BOSS自动激活）自动领养下一周的BOSS。
	add_talk #b#b
	add_talk 灌注说明：
	add_talk #b#b每个公会成员每天可以接受一次灌注任务，完成任务可获得大量的经验。
	add_talk #b#b在不接受任务的情况下，也可以对BOSS灌注能量，每次都会获得少量奖励。

		option 151 下一页（能量来源；激活说明；奖励说明）
		option 1 返回
	talk
endproc

proc 151
	dialog 1
	add_talk 能量来源：
;	add_talk #b#b目前能够对BOSS灌注能量的物品有：
;	add_talk #b#b纯净能量棒（暂时未有出处）
	add_talk #b#b聚焦能量管出处：完成#L宠趣任务@@[306042,4400]#n获得

	add_talk #b#b
	add_talk 激活说明：
	add_talk #b#b#c186506每周日6点-24点之间会长可手动激活BOSS，在此时间段内未激活则BOSS自动激活。激活后BOSS会出现在公会城市中“计划执行官-史明威”附近。召唤后3个小时未击杀，BOSS会消失。

	add_talk #b#b
	add_talk 奖励说明：
	add_talk #b#b击杀BOSS时，附近的公会成员皆会获得经验奖励（可享受双倍经验加成）；BOSS死亡后会掉落BOSS宝箱，由会长开启。BOSS资质越高，奖励越好！
;	add_talk #b#b
	add_talk #b#b

		option 150 上一页（BOSS品质、领养限制、灌注说明）
		option 1 返回
	talk

endproc


proc 200
	strvar Pos
	player_clan_pos_info 1 0 0 0 Pos
	if Pos = 8
		CLAN_BABY_CALL 1
		
	
	else
		add_sys_msg_start
		add_sys_msg 只有会长才可以操作
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
















