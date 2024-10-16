
proc 1
	dialog 1
	add_talk #b#b当你年轻的时候，以为什么都有答案，可是老了的时候，你可能又觉得其实人生并没有所谓的答案。

	option 13 #装备面板
	option 12 #学习技能
	option 11 #再见	
	
	talk
endproc


proc 11
	return
endproc

proc 12
	open_ui 1
endproc

proc 13
	open_ui 5
endproc
