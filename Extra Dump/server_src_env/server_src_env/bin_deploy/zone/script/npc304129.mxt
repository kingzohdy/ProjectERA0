;npc哥伦布

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在这个充满热情与活力的夏天，我将在每周二、周四、周六举办一场所有勇士都能够参与的“航海”比赛，凡是参与的人都能获得奖励，若是能够进入比赛前十名，还能获得非常丰厚的奖励！
    add_talk #b#b
	add_talk #b#b世界是属于勇者的！

	;任务接取时，出现该选项
	task_stat 4500 1 20
	if 20 = 2
		option 101 #给我一艘船，我要去寻找新大陆！
	endif
	
	option 201 #查看上一届航海大亨名单
	option 202 航海大亨奖励说明
		
		TASK_ADD_OPTION 4500 0 100
		
	talk
endproc 


proc 2
	get_trig_task 1 2

	if 2 = 4500 
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b在这个充满热情与活力的夏天，我将在每周二、周四举办一场所有勇士都能够参与的”航海“比赛，凡是参与的人都能获得奖励，若是能够进入比赛前十名，还能获得非常丰厚的奖励！
		add_talk #b#b
		add_talk #b#b世界是属于勇者的！
		
		option 101 #给我一艘船，我要去寻找新大陆！
			
		talk
	endif
endproc

proc 100
	TASK_DO_OPTION 4500 0
endproc

PROC 201
	GET_EVENT_STAT 10 95
	if @10 = 1
	;活动正在进行中
		add_sys_msg_start
		add_sys_msg 现在活动正在进行中，请待活动结束后再来查看排名情况
		set_sys_msg_mode 4 0 0
		msg_info
	else
		OPEN_HANG_HAI_RANGE_UI 1
	endif
ENDPROC

proc 101
	DEL_ROLE_TMP_ITEM 0 0
	
	strvar begin_time
	;记录任务开始时间
	GET_TIME_SEC begin_time
	SET_TASK_VAR 4500 0 32 @begin_time 1
	
	;增加临时机械
	APPLY_TMP_ITEM 1461002 0 1
	
	;传送
	TRANSMIT 4403 23200 32600 0
endproc

proc 202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c6d00d2第1名：幸运罗盘x5
	add_talk #b#b#c6d00d2第2名：幸运罗盘x3
	add_talk #b#b#c6d00d2第3名：幸运罗盘x2
	add_talk #b#b#c6d00d2第4-10名：幸运罗盘x1#n
	add_talk #b#b
	add_talk #b#b到达终点的选手都可开启航海王宝藏，有几率获得：幸运罗盘、赏金令、松珀碎块、炽铜碎块、坚钢碎块、失落的藏宝图、星辉宝钻…
	add_talk #b#b
	add_talk #b#b#c186506开启航海王的宝藏需要“航海王的恩惠”，你可以从盛夏商人闻蔚那里得到它。
	
	option 1 返回
	talk
endproc
