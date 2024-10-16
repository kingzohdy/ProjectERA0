proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b梦与现实真的有区别吗？如果我们能够在梦中得偿所愿，又何必在现实中因失误而悔恨！
	add_talk #b#b这一次，无论如何都要够抵挡住黑暗议会的袭击，为科学研究小组争取到更多的时间撤退。
	add_talk #b#b
	add_talk #b#b#cFF0080建议先组队去盘山道获得齐心协力，经验更多#n
	add_talk #c186506特别提示：
	add_talk #b#b队伍人数越多，你所获得的经验越多
	add_talk #b#b队友等级越低，副本难度越低
	add_talk #b#b经验获得只与个人等级有关，跟队友等级无关
	
	OPTION 201 #进入副本
	
	task_add_option 3317 1 101
	
	talk
endproc 

PROC 101
	task_do_option 3317 1
ENDPROC

proc 201
	OPEN_PWORLD_UI 14
endproc