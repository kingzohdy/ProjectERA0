;神奇玩具屋副本里面的npc啊姆拉
proc 1
	DIALOG 1
	add_talk $_npc$：
	add_talk 泡泡屋诀窍：
	add_talk #b#b当泡泡被打破(杀死)之后，就会变成另外一个泡泡，也就是说，泡泡屋里的泡泡总数量是不会减的!
	add_talk #b#b
	add_talk 以下是泡泡衔变规则：
	add_talk #b#b#b#b#cff0080石头#n → #cff4e00剪刀 #n
	add_talk #b#b#b#b#cff4e00剪刀#n → #c0181f9包袱#n
	add_talk #b#b#b#b#c0181f9包袱#n → #cff0080石头#n
	add_talk #b#b
	add_talk 胜利条件：
	add_talk #b#b当计时结束时，泡泡屋里#c186506所有的泡泡归于同一类型(颜色)
	option 150 #谢谢你的指导

	talk
endproc 

