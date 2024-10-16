;创建事件
PROC 1					
	
	;NEW_DYN_MON #返回值 #怪物DefID #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #第一只怪物的实例ID #阵营索引 #难度 #朝向
	;种2个boss
	;中立为0,绿洲为1,沙漠为2
	;女boss,绿洲
	strvar bossA
	strvar bossACamp
	var bossA 282036
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 5838 5400 0 0 0 2 @bossACamp 0 0

	;男boss,沙漠
	var bossA 282035
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 25957 19875 0 0 0 2 @bossACamp 0 0

	;种4个阻挡
	;绿洲
	;NEW_MON #怪物id #怪物数量 #坐标x #坐标y #随机半径 #巡逻ID #是否可以重生 #返回标志 #第一只怪物的实例ID
	;NEW_MON 230014 4 7003 3532 0 0 0 1 2
	new_mon_by_dir 294008 1 7003 3532 0 0 0 0 1 2

	new_mon_by_dir 294008 1 5048 6978 0 0 0 90 1 2

	;沙漠

	new_mon_by_dir 294008 1 25478 17812 0 0 0 0 1 2

	new_mon_by_dir 294008 1 25100 17799 0 0 0 0 1 2

	new_mon_by_dir 294008 1 23500 19998 0 0 0 90 1 2

	new_mon_by_dir 294008 1 23441 20404 0 0 0 90 1 2
	
	;种2个动态复活点

	var bossA 350046
	;var bossA 350046
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 12754 14761 0 0 0 2 @bossACamp 0 0

	var bossA 350048
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 20947 7188 0 0 0 2 @bossACamp 0 0

	;一个万磁干扰器的旗帜

	var bossA 350050
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 15221 8873 0 0 0 2 @bossACamp 0 0

	;绿洲飞行器
	var bossA 350047
	;NEW_CAMP_NPC #返回值 #NpcID #坐标x #坐标y #方向 #阵营索引 #脚本
	NEW_CAMP_NPC 1 350047 11483 5793 0 1 battleoasisfly.mac

	;沙漠飞行器
	var bossA 350049
	;NEW_CAMP_NPC #返回值 #NpcID #坐标x #坐标y #方向 #阵营索引 #脚本
	NEW_CAMP_NPC 1 350049 18100 18479 0 2 battleoasisfly.mac

	;绿洲传送球
	NEW_CAMP_NPC 1 333038 5929 2851 0 1 battledesertout.mac
	;沙漠传送球
	NEW_CAMP_NPC 1 333039 26107 21261 0 2 battledesertout.mac

	add_sys_msg_start
	add_sys_msg 战斗即将开始
	set_sys_msg_mode 4 0 0
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
	ADD 10 1
	set_pworld_var 0 32 @10 1
	;一分钟后,删阻挡,战斗开始
	if 10 = 30
		;删阻挡
		delete_mon_by_defid 294008 1

		add_sys_msg_start
		add_sys_msg 战斗开始
		set_sys_msg_mode 4 0 0
		map_msg_info

	endif


ENDPROC

PROC 4
	

ENDPROC

;重置战场,调试用
PROC 10
	pworld_again
ENDPROC

;以下为暂时放在这里的脚本

;蓝方飞行器的脚本zcflyblue,
;---需要飞行路径
proc 11
	autofly 106 1
endproc

;红方飞行器的脚本zcflyred,暂时放这
;---需要飞行路径
proc 12
	autofly 107 1
endproc

;传送点脚本,outbattle
PROC 13
        choose 2 10 11 是否要离开当前战场？
ENDPROC

proc 14
        OUT_PWORLD_BY_SINGLE
endproc

proc 15

endproc
