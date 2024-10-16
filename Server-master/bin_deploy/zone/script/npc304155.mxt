;阿姆罗

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b战车实战模拟正在进行中，你现在就需要返回时间城吗？
	add_talk #b#b
	add_talk #b#b#c186506返回时间成后无法再参加该次【轰炸大战】#n
	
	option 100 #返回时间城
	
	talk

endproc

proc 100
	choose 0 101 302 　　　　离开后无法再参加该次【轰炸大战】　　　　是否确认返回时间城？
endproc

proc 101
	transmit 1000 36319 34445 1
endproc
