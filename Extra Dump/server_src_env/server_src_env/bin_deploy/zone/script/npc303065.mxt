proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我这能做到隔壁肌肉男那无法帮你完成的事情，记住了是独一无二的。
	add_talk #b#b#c186506看到装备上的那些【#cff4e00技能属性#c186506】了吗？我能将【#cff4e00技能属性#c186506】替换至另外一件装备上#n
	add_talk #b#b#c005ebe不会改变目标装备上的其他任何属性哦！

		option 110 #【初级技能属性转移】
		option 120 #【高级技能属性转移】
		option 130 #【装备技能属性改造】
	talk

endproc 

proc 110
	open_ui 88
endproc

proc 120
	open_ui 89
endproc

proc 130
	open_ui 95
endproc
