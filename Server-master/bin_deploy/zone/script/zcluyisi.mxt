proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b啊，这位#Race#，一看您就非池中物，再看您身上这一身……呃，宝甲，如果丢了多可惜啊，不如来我这里做个绑定如何？这样您就可以安心地出生入死，杀敌御险，再无后顾之忧。
	add_talk #b#b啊，对了，凡是经过我绑定的东西，只有我才能解绑！
		
		option 150 #装备绑定
;		option 151 #装备解除绑定
	talk

endproc 
proc 150
	open_ui 50
endproc 
proc 151
	open_ui 51
endproc