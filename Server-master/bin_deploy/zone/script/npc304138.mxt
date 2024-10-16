;npc马可波罗

proc 1
	;strvar x y m
	;获取玩家坐标
    ;GET_PLAYER_MAPINFO 1 0 0 x y
	;在这个坐标的6米内都算完成
	;GET_POINT_DIST @x @y 24900 30000 m
	;if m < 600
		;判断是否有临时机械
	;	GET_ROLE_TMP_ITEM 0 10
	;	if @10 = 1461002
	;		task_stat 4500 1 20
	;		if 20 = 2
	;			call 201
	;			task_fini 4500 1			
	;			call 202
	;			DEL_ROLE_TMP_ITEM 0 0
	;		endif
	;	endif
	;endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你穿越了重重迷雾，无数艰难险阻抵达到航海比赛的终点，我猜你心中一定充满着对未知的渴望，对陌生的好奇。保持这份好奇与渴望，$_player$，发现只孕育在永往直前的坚持中。

	;判断是否有临时机械1461002 和身上是否有任务4500  有才出现该选项
    GET_ROLE_TMP_ITEM 0 15
	task_stat 4500 1 16
	if @15 = 1461002 and @16 = 2
		option 205 #航海大亨比赛终点签到
	endif
	option 200 #回到岸上
	
	talk
endproc 

PROC 200
	;传送
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC

;记录完成时间
proc 201
	strvar sec min tmpvar hanghaisec
	;记录任务结束时间
	GET_TIME_SEC sec
	GET_TASK_VAR 4500 0 32 1 15
	
	sub sec @15
	;先记录下来给客户端
	var hanghaisec @sec	
	HANG_HAI_RANGE 1 @hanghaisec
	
	if sec > 60
		var tmpvar @sec
		dev tmpvar 60
		var min @tmpvar
		mod sec 60		
	else
		var min 0 
	endif
	add_sys_msg_start
	add_sys_msg 您本次航海的速度是$@min$分$@sec$秒，请在活动结束后前往哥伦布处查看航海大亨名单
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 205
	call 201
	task_fini 4500 1	
	DEL_ROLE_TMP_ITEM 0 0
endproc
