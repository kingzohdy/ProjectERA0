proc 1
	dialog 1
	ADD_TALK 宠物指令、仓库、公会
	option 12 #仓库
	option 13 #宠物操作
	option 14 #动态副本
	option 15 公会
	option 16 装备加工
	option 17 宝石操作
	option 11 #再见	
	TALK
endproc

proc 11
	return
endproc

proc 12
	open_ui 0
endproc

proc 13
	open_ui 4
endproc

proc 14
	open_pworld_ui 503
endproc

proc 15
	dialog 1
	ADD_TALK 公会操作
	option 51 基础操作
	option 52 公会/城市/建筑升级面板
	option 39 #进入城市
	option 40 其他操作
	option 1 #返回
	TALK
endproc

proc 22
	open_ui 6
endproc

proc 23
	open_ui 7
endproc

proc 24
	open_ui 8
endproc

proc 25
	open_ui 9
endproc

proc 16
	dialog 1
	ADD_TALK 装备加工
	option 32 #装备品质提升
	option 33 #装备战斗力提升
	option 34 #装备等级提升
	option 35 #装备鉴定
	option 1 返回
	TALK
endproc

proc 17
	dialog 1
	ADD_TALK 宝石操作
	option 36 #宝石升级
	option 37 #宝石镶嵌
	option 38 #宝石摘取
	option 1 返回
	TALK
endproc

proc 32
	open_ui 10
endproc

proc 33
	open_ui 11
endproc

proc 34
	open_ui 12
endproc

proc 35
	open_ui 13
endproc

proc 36
	open_ui 14
endproc

proc 37
	open_ui 15
endproc

proc 38
	open_ui 16
endproc

proc 39
	enter_clan_city
endproc

proc 40
	dialog 1
	ADD_TALK 公会其他功能
	option 41 公会任务组
	option 42 城市任务组
	; option 45 #发放绑定天元
	; option 46 #发放经验值
	option 47 调节公会任务组
	option 48 调节城市任务组
	TALK
endproc

proc 41
	dialog 1
	ADD_TALK 当前公会任务组
	dynamic_option_list 0 1 43
	TALK
endproc

proc 42
	dialog 1
	ADD_TALK 当前公会城市任务组
	dynamic_option_list 1 1 44
	TALK
endproc

proc 43
	dynamic_do_task_option 41
endproc

proc 44
	dynamic_do_task_option 42
endproc

; proc 45
	; clan_pay 0 1
; endproc

; proc 46
	; clan_pay 1 1
; endproc

proc 47
	dialog 1
	ADD_TALK 当前可选公会任务组
	dynamic_option_list 2 1 49
	TALK
endproc

proc 48
	dialog 1
	ADD_TALK 当前可选公会城市任务组
	dynamic_option_list 3 1 50
	TALK
endproc

proc 49
	dynamic_do_task_option 40
endproc

proc 50
	dynamic_do_task_option 40
endproc

proc 51
	dialog 1
	ADD_TALK 公会基础操作
	option 22 #创建公会
	option 23 #解散公会
	option 24 #搜索公会
	option 25 #修改宣告
	option 15 返回
	TALK
endproc

proc 52
	dialog 1
	ADD_TALK 公会/城市/建筑升级面板
	option 53 #升级公会面板
	option 54 #升级公会城市面板
	option 55 #升级行政局面板
	option 56 #升级财务局面板
	option 57 #升级兵工厂面板
	option 58 #升级科技局面板
	option 1 #返回
	TALK
endproc

proc 53
	open_ui 20
endproc

proc 54
	open_ui 18
endproc

proc 55
	open_ui 21
endproc

proc 56
	open_ui 22
endproc

proc 57
	open_ui 23
endproc

proc 58
	open_ui 24
endproc
