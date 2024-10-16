;npc 希恩  领取潜伏任务

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b无论什么时候，只要自由的领土再次受到侵犯，我将不惜一切代价，穿起军装，拿起武器，即使我已垂垂老矣，用鲜血与生命去守护每一个向往自由的联盟子民。
	;add_talk #b#b记忆扫描器可分辨黑暗文明议会初级间谍（若当前区域队伍成员数量≥4人，就会召唤出高级间谍）
	;add_talk #b#b击杀高级间谍可获得#186506英雄纪念币#n
	add_talk #b#b
	add_talk #b#b#cff0080每天最多可接受30次潜伏任务，放弃任务消耗次数。#n
	
		TASK_ADD_OPTION 4635 0 100
		
		TASKCYCLE_ADD_OPTION 147 1 110
	talk
endproc 

proc 100
	TASK_DO_OPTION 4635 0
endproc

PROC 110	
	TASKCYCLE_DO_OPTION 147 1
ENDPROC
