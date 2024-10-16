;未开垦杂草

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只需要花费银币就可以使你的菜园变得更大，种植更多的植被！
	add_talk #b#b你要开垦这块土地，让你的菜园更广阔吗？
	add_talk #b#b
	add_talk #b#b#c186506开垦费用：10000银币#n
	
		option 100 开垦菜园
	
	talk
endproc 

proc 100
	home_kaiken 0
endproc
