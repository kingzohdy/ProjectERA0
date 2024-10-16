;304124

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b“只有经历过岁月磨砺的人生，才足以拥有引导他人的资格”正是艾吉奥先生提出的这个理念，创办了这所【皇家学院】。
	add_talk #b#b学院由始自终秉承着这种态度，我们坚信唯有身经百战的人，才能更准确有效的为迷航者照亮前进的方向。
	add_talk #b#b为此我们将以最诚意的支持与鼓励，希望有更多的勇士能够成为指明他人的导师。

	OPTION 200 #(买卖)打开商店
	OPTION 300 我想了解【传承点数】

	talk
endproc

proc 200
	 OPEN_NPC_SHOP
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你愿意，我们可以详细聊聊【#cff4e00传承点数#n】。
	
	OPTION 301 【传承点数】有什么用？
	OPTION 302 【传承点数】怎么获取？
	OPTION 303 【传承点数】在哪查看？
	OPTION 1 我已经都明白了
	
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不要小视【#cff4e00传承点数#n】的用途。
	add_talk #b#b你只要支付【#cff4e00传承点数#n】，即可从我这里购买到特有的【#c186506学院徽章#n】，或是其它罕有宝物。
	
	OPTION 300 我想了解更多
	
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	; add_talk #b#b【#cff4e00传承点数#n】共有好几种的获取方式。
	; add_talk #b#b只要在我这购买【#c186506失落图腾#n】，击杀【失落之域】的怪物蓄满图腾经验值后，即可获得【#cff4e00传承点数#n】。
	; add_talk #b#b
	add_talk #b#b如果你已建立了【师生】关系，并且成为了一名【#c186506导师#n】。
	add_talk #b#b可以在【好友】面板的【师徒】页面中打开【师生心得】面板，向你的【学生】传授经验值来获得【#cff4e00传承点数#n】。
	add_talk #b#b也可以协助你的【学生】通关任意【困难】或【专家】副本来获得【#cff4e00传承点数#n】。
	
	OPTION 300 我想了解更多
	
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b【#cff4e00传承点数#n】的查看方式其实很简单。
	add_talk #b#b你可以通过【快捷键#c186506C#n】打开【人物】面板，点开面板下方的【声望】页面查看自己有多少【#cff4e00传承点数#n】。
	
	OPTION 300 我想了解更多
	
	talk
endproc
	