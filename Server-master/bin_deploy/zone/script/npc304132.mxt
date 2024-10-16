
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在这个充满热情与活力的夏天，我将在每周二、周四、周六举办一场所有勇士都能够参与的“航海”比赛，凡是参与的人都能获得奖励，若是能够进入比赛前十名，还能获得非常丰厚的奖励！
    add_talk #b#b
	add_talk #b#b世界是属于勇者的！
	add_talk #b#b
	add_talk #b#b#c186506航海大亨比赛日程表：
	add_talk #b#b#c186506周二、周四、周六 8：00-20：00
	
	option 201 #查看上一届航海大亨名单
			
	talk
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
