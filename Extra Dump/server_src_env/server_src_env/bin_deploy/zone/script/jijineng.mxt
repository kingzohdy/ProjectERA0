	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b拿根棍子就想出去战斗么，没有技能，就是拿着绝世好“剑”你也“剑”不动。
	
	option 12 #学习技能

	talk


endproc

proc 12
	open_ui 1
endproc