;新兵签到牌

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b各位自治联盟的居民，自零纪元640年以来，末世帝国对我们发动了数次进攻，造成了少量伤亡，大量被俘。由于我们现在兵源十分短缺，为了保卫我们的家园，为了让我们重新拥有幸福和平的世界，加入我们吧！

	TASK_STAT 1051 1 3
	if 3 = 2
		option 100 #签到
	endif
	talk
endproc

proc 100
	strvar X Y
	GET_PLAYER_MAPINFO 0 0 0 X Y
	
	effect 2 0 playUiEffect(0,0,2,"effect\\entity\\9576_16.ent")	
	
	effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9576_18.ent")
	TASK_FINI 1051 0
	
endproc 


