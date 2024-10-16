;妮妮

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b自从【虚拟体·伽马】背叛后，便将我禁锢在此。
	add_talk #b#b这次我重新获得自由，想必修斯佩恩已被再次封印。
	add_talk #b#b
	add_talk #b每次击杀修斯佩恩有机会出现#c005ebe可捕捉宠物-妮妮#n
	
		option 300 #希望你能和我并肩作战！（捕捉诀窍）
		option 200 #你和【虚拟体·伽马】是什么关系
	
	
	talk
endproc

proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b曾经我狂热于科学研发，伽马就是我研发的拟人体高智能机器人。
	add_talk #b#b为了创造出最强的机器人，我不断把自己的力量分送给伽马。
	add_talk #b#b可随着我越发沉迷，伽马的力量也随之迅速膨胀，直到我无法再驾驭这力量。
	add_talk #b#b后来，黑暗议会的人来了。
	add_talk #b#b为了获得更强的力量，伽马与他们勾结把我禁锢在此，希望利用我的力量扰乱修斯佩恩的封印。
	add_talk #b#b
	add_talk #b#b#c186506如今伽马就藏匿在【远古要塞】，希望你能替我破坏这个邪恶的智能体。#n
	
		option 1 #返回
	
	talk
endproc

proc 300
	DIALOG 1
	add_talk #b每次击杀修斯佩恩有机会出现#c005ebe可捕捉宠物-妮妮#n
	add_talk #b#b
	add_talk #c186506捕捉诀窍：
	add_talk 1. 使用“困兽网”捕捉，可提升成功几率
	add_talk 2. “妮妮”生命值越低，捕捉成功率越高#n
	
		option 1 #返回
	
	talk
endproc
